/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMultiLinearCondition.cpp
@Time: 2023/6/16 22:09
@Desc: 
***************************/

#include "GMultiLinearCondition.h"

CGRAPH_NAMESPACE_BEGIN

GMultiLinearCondition::GMultiLinearCondition() {
    element_type_ = GElementType::LINEAR_CONDITION;
    session_ = URandom<>::generateSession(CGRAPH_STR_LINEAR_CONDITION);
}


CStatus GMultiLinearCondition::run() {
    CGRAPH_FUNCTION_BEGIN

    for (auto cur: this->group_elements_arr_) {
        if (cur->isMatch()) {
            // 如果在
            status = cur->fatProcessor(CFunctionType::RUN);
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    CGRAPH_FUNCTION_END
}


CIndex GMultiLinearCondition::choose() {
    CGRAPH_THROW_EXCEPTION("GMultiLinearCondition no support choose function")
    return 0;
}

CGRAPH_NAMESPACE_END


