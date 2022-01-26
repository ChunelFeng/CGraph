/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCondition.cpp
@Time: 2021/6/19 5:00 下午
@Desc: 
***************************/

#include "GCondition.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GCondition::init() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : this->condition_elements_) {
        status = element->init();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CStatus GCondition::destroy() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : this->condition_elements_) {
        status = element->destroy();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CStatus GCondition::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    this->condition_elements_.emplace_back(element);
    CGRAPH_FUNCTION_END
}


CStatus GCondition::run() {
    CGRAPH_FUNCTION_BEGIN

    int loop = 0;
    int index = this->choose();
    if (GROUP_LAST_ELEMENT_INDEX == index
        && !this->condition_elements_.empty()) {
        // 如果返回-1，则直接执行最后一个条件（模仿default功能）
        loop = condition_elements_.back()->loop_;
        while (loop-- > 0) {
            status = condition_elements_.back()->run();
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    } else if (0 <= index && index < condition_elements_.size()) {
        // 如果返回的内容，在元素范围之内，则直接执行元素的内容。不在的话，则不执行任何操作，直接返回正确状态
        loop = condition_elements_[index]->loop_;
        while (loop-- > 0) {
            status = condition_elements_[index]->run();
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    CGRAPH_FUNCTION_END
}


int GCondition::getRange() const {
    return (int)condition_elements_.size();
}

CGRAPH_NAMESPACE_END
