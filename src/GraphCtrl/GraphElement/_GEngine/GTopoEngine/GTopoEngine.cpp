/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GTopoEngine.cpp
@Time: 2024/1/20 21:56
@Desc: 
***************************/

#include "GTopoEngine.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GTopoEngine::setup(const GSortedGElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN

    topo_elements_ = GEngine::getTopo(elements);
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

CGRAPH_NAMESPACE_END
