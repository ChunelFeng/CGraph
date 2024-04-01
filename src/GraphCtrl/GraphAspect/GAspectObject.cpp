/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectObject.cpp
@Time: 2024/4/1 21:53
@Desc: 
***************************/

#include "GAspectObject.h"
#include "../GraphElement/GElementInclude.h"

CGRAPH_NAMESPACE_BEGIN

GAspectObject::GAspectObject() {
    session_ = URandom<>::generateSession(CGRAPH_STR_ASPECT);
}


const std::string& GAspectObject::getName() const {
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(belong_);
    return belong_->getName();
}


GAspectObject::~GAspectObject() {
    CGRAPH_DELETE_PTR(param_)
}


auto GAspectObject::setBelong(GElementPtr belong)
-> decltype(this) {
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(belong)
    belong_ = belong;
    return this;
}

CGRAPH_NAMESPACE_END