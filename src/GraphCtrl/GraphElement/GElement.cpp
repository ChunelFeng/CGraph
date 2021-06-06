/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElement.cpp
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

#include <uuid/uuid.h>
#include "GElement.h"

/******** public ********/
/* 获取name信息 */
std::string GElement::getName() const {
    return this->name_;
}


/* 获取session信息（全局唯一） */
std::string GElement::getSession() const {
    return this->session_;
}


/******** protected ********/
GElement::GElement() {
    this->session_ = _generateSession();
}


GElement::~GElement() {

}


GElement::GElement(const GElement& element) {
    this->done_ = element.done_;
    this->is_init_ = element.is_init_;
    this->run_before_ = element.run_before_;
    this->dependence_ = element.dependence_;
    this->left_depend_.store(element.left_depend_);
    this->linkable_ = element.linkable_;
    this->session_ = element.session_;
    this->loop_ = element.loop_;
    this->name_ = element.name_;
}


GElement& GElement::operator=(const GElement& element) {
    if (this == &element) {
        return *this;
    }

    this->done_ = element.done_;
    this->is_init_ = element.is_init_;
    this->run_before_ = element.run_before_;
    this->dependence_ = element.dependence_;
    this->left_depend_.store(element.left_depend_);
    this->linkable_ = element.linkable_;
    this->session_ = element.session_;
    this->loop_ = element.loop_;
    this->name_ = element.name_;

    return *this;
}


void GElement::setName(const std::string& name) {
    if (likely(!name.empty())) {
        this->name_ = name;
    } else {
        this->name_ = this->session_;
    }
}


void GElement::setLoop(int loop) {
    /* 至少执行一次 */
    this->loop_ = (loop >= 0) ? loop : 0;
}


bool GElement::isRunnable() const {
    return 0 >= this->left_depend_ && false == this->done_;
}


bool GElement::isLinkable() const {
    return this->linkable_;
}


CSTATUS GElement::process(bool isMock) {
    CGRAPH_PROCESS_ERROR
}


std::string GElement::_generateSession() {
    uuid_t uuid;
    char session[36] = {0};    // 36是特定值
    uuid_generate(uuid);
    uuid_unparse(uuid, session);

    return session;
}