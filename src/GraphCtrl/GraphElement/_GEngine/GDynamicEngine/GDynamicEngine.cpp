/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GDynamicEngine.cpp
@Time: 2022/12/16 22:46 下午
@Desc: 
***************************/

#include "GDynamicEngine.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GDynamicEngine::setup(const GSortedGElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN
    link(elements);

    // 给所有的值清空
    total_element_arr_.clear();
    front_element_arr_.clear();
    total_end_size_ = 0;

    // 确定所有的信息
    for (GElementPtr element : elements) {
        CGRAPH_ASSERT_NOT_NULL(element)
        if (element->run_before_.empty()) {
            total_end_size_++;
        }

        if (element->dependence_.empty()) {
            front_element_arr_.emplace_back(element);
        }
        total_element_arr_.emplace_back(element);
    }

    CGRAPH_FUNCTION_END
}


CStatus GDynamicEngine::run() {
    CGRAPH_FUNCTION_BEGIN

    if (likely(total_end_size_ != total_element_arr_.size())) {
        beforeRun();
        asyncRunAndWait();
    } else {
        parallelRunAll();
    }
    status = cur_status_;
    CGRAPH_FUNCTION_END
}


CStatus GDynamicEngine::afterRunCheck() {
    CGRAPH_FUNCTION_BEGIN
    if (likely(total_end_size_ != total_element_arr_.size())) {
        /**
         * 非纯并行的逻辑下，才需要判断。
         * 纯并行度逻辑，肯定会所有都跑一遍，并且等待全部结束，
         * 故，不需要判断。
         */
        for (GElementCPtr element : total_element_arr_) {
            CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!element->done_,    \
                                                    element->getName() + ": dynamic engine, check not run it...")
        }
    }

    CGRAPH_FUNCTION_END
}


CVoid GDynamicEngine::asyncRunAndWait() {
    /**
     * 1. 执行没有任何依赖的element
     * 2. 在element执行完成之后，进行裂变，直到所有的element执行完成
     * 3. 等待异步执行结束
     */
    for (const auto& element : front_element_arr_) {
        process(element, element == front_element_arr_.back());
    }

    wait();
}


CVoid GDynamicEngine::beforeRun() {
    finished_end_size_ = 0;
    cur_status_.reset();
    for (GElementPtr element : total_element_arr_) {
        element->beforeRun();
    }
}


CVoid GDynamicEngine::process(GElementPtr element, CBool affinity) {
    if (unlikely(cur_status_.isErr() || element->done_)) {
        /**
         * 如果已经有异常逻辑，
         * 或者传入的element，是已经执行过的了（理论上不会出现这种情况，由于提升性能的原因，取消了atomic计数的逻辑，故添加这一处判定，防止意外情况）
         * 则直接停止当前流程
         */
        return;
    }

    const auto& exec = [this, element] {
        const CStatus& curStatus = element->fatProcessor(CFunctionType::RUN);
        if (unlikely(curStatus.isErr())) {
            // 当且仅当整体状正常，且当前状态异常的时候，进入赋值逻辑。确保不重复赋值
            cur_status_ += curStatus;
        }
        afterElementRun(element);
    };

    if (affinity
        && CGRAPH_DEFAULT_BINDING_INDEX == element->getBindingIndex()) {
        // 如果 affinity=true，表示用当前的线程，执行这个逻辑。以便增加亲和性
        exec();
    } else {
        thread_pool_->commit(exec, calcIndex(element));
    }
}


CVoid GDynamicEngine::afterElementRun(GElementPtr element) {
    element->done_ = true;
    if (!element->run_before_.empty() && cur_status_.isOK()) {
        auto curSize = element->run_before_.size();
        if (1 == curSize && (*element->run_before_.begin())->isLinkable()) {
            // 针对linkable 的情况，做特殊判定
            process(*(element->run_before_.begin()), true);
        } else {
            GElementPtr reserved = nullptr;
            for (auto* cur : element->run_before_) {
                if (--cur->left_depend_ <= 0) {
                    /**
                     * 留下来一个作为亲和性的，在接下来的流程中执行
                     * 其他的，走 else 后面的逻辑，直接在 process 中异步执行
                     */
                    if (!reserved) {
                        reserved = cur;
                    } else {
                        process(cur, false);
                    }
                }
            }
            reserved ? process(reserved, true) : void();
        }
    } else {
        CGRAPH_LOCK_GUARD lock(lock_);
        /**
         * 满足一下条件之一，则通知wait函数停止等待
         * 1，无后缀节点全部执行完毕(在运行正常的情况下，只有无后缀节点执行完成的时候，才可能整体运行结束)
         * 2，有节点执行状态异常
         */
        if ((element->run_before_.empty() && (++finished_end_size_ >= total_end_size_))
            || cur_status_.isErr()) {
            cv_.notify_one();
        }
    }
}


CVoid GDynamicEngine::wait() {
    CGRAPH_UNIQUE_LOCK lock(lock_);
    cv_.wait(lock, [this] {
        /**
         * 遇到以下条件之一，结束执行：
         * 1，执行结束
         * 2，状态异常
         */
        return (finished_end_size_ >= total_end_size_) || cur_status_.isErr();
    });
}


CVoid GDynamicEngine::parallelRunAll() {
    /** 特殊判定：如果只有一个节点的话，则直接执行就好了 */
    if (1 == total_end_size_) {
        cur_status_ += front_element_arr_[0]->fatProcessor(CFunctionType::RUN);
        return;
    }

    /**
     * 主要适用于dag是纯并发逻辑的情况
     * 直接并发的执行所有的流程，从而减少调度损耗
     * 实测效果，在32路纯并行的情况下，整体耗时从 21.5s降低到 12.5s
     * 非纯并行逻辑，不走此函数
     */
    std::vector<std::future<CStatus>> futures;
    futures.reserve(total_end_size_);
    for (auto* element : front_element_arr_) {
        futures.emplace_back(thread_pool_->commit([element] {
            return element->fatProcessor(CFunctionType::RUN);
        }, calcIndex(element)));
    }

    for (auto& fut : futures) {
        cur_status_ += fut.get();
    }
}

CGRAPH_NAMESPACE_END
