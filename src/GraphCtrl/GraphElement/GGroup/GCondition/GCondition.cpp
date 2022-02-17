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
        /** init和destroy的时候，切面全部执行。run的时候，仅执行被执行的切面 */
        status = element->fatProcessor(CFunctionType::INIT);
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CStatus GCondition::destroy() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : this->condition_elements_) {
        status = element->fatProcessor(CFunctionType::DESTROY);
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

    CSize loop = 0;
    CIndex index = this->choose();
    if (GROUP_LAST_ELEMENT_INDEX == index
        && !this->condition_elements_.empty()) {
        // 如果返回-1，则直接执行最后一个条件（模仿default功能）
        loop = condition_elements_.back()->loop_;
        while (loop-- > 0) {
            auto element = condition_elements_.back();
            status = element->fatProcessor(CFunctionType::RUN);
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    } else if (0 <= index && index < condition_elements_.size()) {
        // 如果返回的内容，在元素范围之内，则直接执行元素的内容。不在的话，则不执行任何操作，直接返回正确状态
        loop = condition_elements_[index]->loop_;
        while (loop-- > 0) {
            auto element = condition_elements_[index];
            status = element->fatProcessor(CFunctionType::RUN);
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    CGRAPH_FUNCTION_END
}


CSize GCondition::getRange() const {
    return (CSize)condition_elements_.size();
}

CGRAPH_NAMESPACE_END
