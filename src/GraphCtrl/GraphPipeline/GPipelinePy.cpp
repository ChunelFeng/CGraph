/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPipelinePy.cpp
@Time: 2023/4/2 22:41
@Desc: 
***************************/

#include "GPipelinePy.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GPipelinePy::registerGElement(GElementPtr element,
                                      const GElementPtrSet &dependElements,
                                      const std::string &name,
                                      CSize loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    status = element->setElementInfo(dependElements, name, loop,
                                     this->param_manager_,
                                     this->event_manager_);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->add(element);
    CGRAPH_FUNCTION_CHECK_STATUS
    element_repository_.insert(element);

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
