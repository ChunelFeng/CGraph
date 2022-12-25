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

    end_size_ = 0;
    for (GElementPtr element : elements) {
        if (element->run_before_.empty()) {
            end_size_++;
        }
    }
    manager_elements_ = elements;

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

    for (GElementPtr element : manager_elements_) {
        if (!element->done_) {
            CGRAPH_RETURN_ERROR_STATUS("pipeline run check failed...");
        }
    }

    CGRAPH_FUNCTION_END
}


CVoid GDynamicEngine::asyncRun() {
    for (const auto& element : manager_elements_) {
        if (element->dependence_.empty() && !element->done_) {
            runElementTask(element);
        }
    }
}


CStatus GDynamicEngine::beforeRun() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : manager_elements_) {
        element->done_ = false;
        status = element->beforeRun();
        CGRAPH_FUNCTION_CHECK_STATUS
    }
    finish_end_size = 0;

    CGRAPH_FUNCTION_END
}


CVoid GDynamicEngine::runElementTask(GElementPtr element) {
    thread_pool_->commit(std::bind(&GDynamicEngine::runElement, this, element));
}


CVoid GDynamicEngine::runElement(GElementPtr element) {
    element->fatProcessor(CFunctionType::RUN);
    afterElementRun(element);
}


CVoid GDynamicEngine::afterElementRun(GElementPtr element) {
    element->done_ = true;

    for(auto cur : element->run_before_) {
        bool runnable = (0 == (--cur->left_depend_));
        if (!runnable) continue;

        runElementTask(cur);
    }

    if (element->run_before_.empty()) {
        this->increaseEnd();    // 计算执行结束节点数量
    }
}


CVoid GDynamicEngine::wait() {
    CGRAPH_UNIQUE_LOCK lock(lock_);

    while(finish_end_size < end_size_) {
        cv_.wait(lock);
    }
}


CVoid GDynamicEngine::increaseEnd() {
    CGRAPH_UNIQUE_LOCK lock(lock_);
    ++finish_end_size;
    if (finish_end_size >= end_size_) {
        finish_end_size = end_size_;
        cv_.notify_one();
    }
}

CGRAPH_NAMESPACE_END
