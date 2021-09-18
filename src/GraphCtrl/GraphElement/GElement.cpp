/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElement.cpp
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

#include "GElement.h"

/******** public ********/
/* 获取name信息 */
const std::string& GElement::getName() const {
    return this->name_;
}


/* 获取session信息（全局唯一） */
const std::string& GElement::getSession() const {
    return this->session_;
}


/******** protected ********/
GElement::GElement() {
    this->session_ = CGRAPH_GENERATE_SESSION();
}


GElement::~GElement() = default;


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
    if (!name.empty()) {
        this->name_ = name;
    } else {
        this->name_ = this->session_;
    }
}


void GElement::setLoop(int loop) {
    this->loop_ = (loop >= 0) ? loop : 0;
}


bool GElement::isRunnable() const {
    return 0 >= this->left_depend_ && !this->done_;
}


bool GElement::isLinkable() const {
    return this->linkable_;
}


CSTATUS GElement::process(bool isMock) {
    CGRAPH_NO_SUPPORT
}


CSTATUS GElement::setParamManager(GParamManagerPtr manager) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(manager)
    CGRAPH_ASSERT_INIT(false)

    this->param_manager_ = manager;

    CGRAPH_FUNCTION_END
}
