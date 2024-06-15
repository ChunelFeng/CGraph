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


GAspectObjectPtr GAspectObject::setBelong(GElementPtr belong) {
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(belong)
    CGRAPH_THROW_EXCEPTION_BY_CONDITION(!belong->isRegistered(), \
                                        "[" + belong->getName() + "] can not add aspect for the reason of no register");
    belong_ = belong;
    this->setGParamManager(belong->param_manager_);
    this->setGEventManager(belong->event_manager_);
    return this;
}

CGRAPH_NAMESPACE_END