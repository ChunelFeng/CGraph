/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GDynamicEngine.cpp
@Time: 2022/12/16 22:46 下午
@Desc: 
***************************/

#include "GDynamicEngine.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GDynamicEngine::setUp(const GSortedGElementPtrSet& elements) {
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

    wait();
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
    for (GElementPtr element : total_element_arr_) {
        status += element->beforeRun();
    }

    CGRAPH_FUNCTION_END
}


CStatus GDynamicEngine::process(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    thread_pool_->commit([this, element] {
        element->fatProcessor(CFunctionType::RUN);
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

    if (element->run_before_.empty()) {
        this->checkFinishState();    // 计算执行结束节点数量
    }
}


CVoid GDynamicEngine::wait() {
    CGRAPH_UNIQUE_LOCK lock(lock_);
    while (finished_end_size_ < total_end_size_) {
        cv_.wait(lock);
    }
}


CVoid GDynamicEngine::checkFinishState() {
    CGRAPH_UNIQUE_LOCK lock(lock_);
    if (++finished_end_size_ >= total_end_size_) {
        cv_.notify_one();
    }
}

CGRAPH_NAMESPACE_END
