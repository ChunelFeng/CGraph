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
    status = beforeRun();
    CGRAPH_FUNCTION_CHECK_STATUS

    asyncRun();

    status = wait();
    CGRAPH_FUNCTION_END
}


CStatus GDynamicEngine::afterRunCheck() {
    CGRAPH_FUNCTION_BEGIN
    if (run_element_size_ != total_element_arr_.size()) {
        CGRAPH_RETURN_ERROR_STATUS("pipeline run element size not match...")
    }

    for (GElementPtr element : total_element_arr_) {
        if (!element->done_) {
            CGRAPH_RETURN_ERROR_STATUS("pipeline run check failed...")
        }
    }

    CGRAPH_FUNCTION_END
}


CVoid GDynamicEngine::asyncRun() {
    for (const auto& element : front_element_arr_) {
        process(element);
    }
}


CStatus GDynamicEngine::beforeRun() {
    CGRAPH_FUNCTION_BEGIN

    finished_end_size_ = 0;
    run_element_size_ = 0;
    cur_status_.reset();
    for (GElementPtr element : total_element_arr_) {
        status += element->beforeRun();
    }

    CGRAPH_FUNCTION_END
}


CStatus GDynamicEngine::process(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    if (unlikely(cur_status_.isErr())) {
        // 如果当前整体状态异常，直接返回，不执行了
        CGRAPH_RETURN_ERROR_STATUS("current status error")
    }

    thread_pool_->commit([this, element] {
        // 如果当前已经是err状态，则不再执行任何逻辑
        auto curStatus = element->fatProcessor(CFunctionType::RUN);
        if (unlikely(curStatus.isNotOK() && cur_status_.isNotErr())) {
            // 当且仅当整体状正常，且当前状态异常的时候，进入赋值逻辑。确保不重复赋值
            cur_status_ = curStatus;
        }
        afterElementRun(element);
    }, this->schedule_strategy_);

    CGRAPH_FUNCTION_END
}


CVoid GDynamicEngine::afterElementRun(GElementPtr element) {
    element->done_ = true;
    run_element_size_++;
    for (auto cur : element->run_before_) {
        if (--cur->left_depend_ <= 0) {
            process(cur);
        }
    }

    CGRAPH_UNIQUE_LOCK lock(lock_);
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


CStatus GDynamicEngine::wait() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_UNIQUE_LOCK lock(lock_);
    cv_.wait(lock,  [this] {
        /**
         * 遇到以下条件之一，结束执行：
         * 1，执行结束
         * 2，状态异常
         */
        return (finished_end_size_ >= total_end_size_) || cur_status_.isErr();
    });
    status = cur_status_;    // 等待结束后，做赋值
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
