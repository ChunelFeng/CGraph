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
    beforeRun();
    asyncRunAndWait();

    status = cur_status_;
    CGRAPH_FUNCTION_END
}


CStatus GDynamicEngine::afterRunCheck() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(run_element_size_ != total_element_arr_.size(),    \
                                            "dynamic engine run element size not match...")
    for (GElementPtr element : total_element_arr_) {
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!element->done_, "dynamic engine run check failed...")
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
    run_element_size_ = 0;
    cur_status_.reset();
    for (GElementPtr element : total_element_arr_) {
        element->beforeRun();
    }
}


CStatus GDynamicEngine::process(GElementPtr element, CBool affinity) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(cur_status_.isErr(), "current status error");

    const auto& exec = [this, element] {
        auto curStatus = element->fatProcessor(CFunctionType::RUN);
        if (unlikely(curStatus.isErr())) {
            // 当且仅当整体状正常，且当前状态异常的时候，进入赋值逻辑。确保不重复赋值
            cur_status_ = curStatus;
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

    CGRAPH_FUNCTION_END
}


CVoid GDynamicEngine::afterElementRun(GElementPtr element) {
    element->done_ = true;
    run_element_size_++;

    std::vector<GElementPtr> ready;    // 表示可以执行的列表信息
    for (auto* cur : element->run_before_) {
        if (--cur->left_depend_ <= 0) {
            ready.emplace_back(cur);
        }
    }

    for (auto& cur : ready) {
        process(cur, cur == ready.back());
    }

    if (!element->run_before_.empty() && cur_status_.isOK()) {
        /**
         * 为了减少进入下方lock的概率
         * 如果不是尾部节点，或者没有执行错误，则不进入下面的逻辑
         */
        return;
    }

    CGRAPH_LOCK_GUARD lock(lock_);
    /**
     * 满足一下条件之一，则通知wait函数停止等待
     * 1，无后缀节点全部执行完毕
     * 2，有节点执行状态异常
     */
    if ((element->run_before_.empty() && (++finished_end_size_ >= total_end_size_))
        || cur_status_.isErr()) {
        cv_.notify_one();
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

CGRAPH_NAMESPACE_END
