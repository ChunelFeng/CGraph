/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GTopoEngine.cpp
@Time: 2024/1/20 21:56
@Desc: 
***************************/

#include <queue>

#include "GTopoEngine.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GTopoEngine::setup(const GSortedGElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN

    topo_elements_.clear();
    std::queue<GElementPtr> readyQueue;
    for (auto* element : elements) {
        element->left_depend_ = element->dependence_.size();
        if (0 == element->left_depend_) {
            readyQueue.push(element);
        }
    }

    while(!readyQueue.empty()) {
        auto* cur = readyQueue.front();
        readyQueue.pop();
        topo_elements_.push_back(cur);

        for (auto* element : cur->run_before_) {
            if (0 == --element->left_depend_) {
                readyQueue.push(element);
            }
        }
    }

    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(topo_elements_.size() != elements.size(),     \
                                            "topo engine parse size is not right")

    CGRAPH_FUNCTION_END
}


CStatus GTopoEngine::run() {
    CGRAPH_FUNCTION_BEGIN
    for (auto* element : topo_elements_) {
        status += element->fatProcessor(CFunctionType::RUN);
        CGRAPH_FUNCTION_CHECK_STATUS
    }
    CGRAPH_FUNCTION_END
}


CStatus GTopoEngine::afterRunCheck() {
    /**
     * topology 纯靠顺序执行。如果有异常，会直接爆出
     * 故，不在这里做判定了
     */
    CGRAPH_EMPTY_FUNCTION
}

CGRAPH_NAMESPACE_END
