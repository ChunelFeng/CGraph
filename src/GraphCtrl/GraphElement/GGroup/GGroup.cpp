/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GGroup.cpp
@Time: 2022/10/16 01:06
@Desc: 
***************************/

#include "GGroup.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GGroup::init() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : group_elements_arr_) {
        CGRAPH_ASSERT_NOT_NULL(element)
        status += element->fatProcessor(CFunctionType::INIT);
    }
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CStatus GGroup::destroy() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : group_elements_arr_) {
        CGRAPH_ASSERT_NOT_NULL(element)
        status += element->fatProcessor(CFunctionType::DESTROY);
    }
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END