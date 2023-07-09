/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GFunction.cpp
@Time: 2023/7/1 00:13
@Desc: 
***************************/

#include "GFunction.h"

CGRAPH_NAMESPACE_BEGIN

GFunctionPtr GFunction::setFunction(const CFunctionType& type,
                                    CGRAPH_CSTATUS_CONST_FUNCTION_REF func) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(func)

    switch (type) {
        case CFunctionType::INIT: init_function_ = func; break;
        case CFunctionType::RUN: run_function_ = func; break;
        case CFunctionType::DESTROY: destroy_function_ = func; break;
        default: return nullptr;     // 不可能出现的情况
    }

    return this;
}


GFunction::GFunction() {
    this->element_type_ = GElementType::FUNCTION;
    session_ = URandom<>::generateSession(CGRAPH_STR_FUNCTION);
}


CStatus GFunction::init() {
    return init_function_ ? init_function_() : CStatus();
}


CStatus GFunction::run() {
    return run_function_ ? run_function_() : CStatus();
}


CStatus GFunction::destroy() {
    return destroy_function_ ? destroy_function_() : CStatus();
}

CGRAPH_NAMESPACE_END