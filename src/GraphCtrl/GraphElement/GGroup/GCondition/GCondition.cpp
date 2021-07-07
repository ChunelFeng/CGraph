/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCondition.cpp
@Time: 2021/6/19 5:00 下午
@Desc: 
***************************/

#include <algorithm>
#include "GCondition.h"

CSTATUS GCondition::init() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : this->condition_elements_) {
        status = element->init();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CSTATUS GCondition::deinit() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : this->condition_elements_) {
        status = element->deinit();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CSTATUS GCondition::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    this->condition_elements_.emplace_back(element);
    CGRAPH_FUNCTION_END
}


CSTATUS GCondition::process(bool isMock) {
    CGRAPH_FUNCTION_BEGIN

    status = this->beforeRun();
    CGRAPH_FUNCTION_CHECK_STATUS

    if (!isMock) {
        status = run();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    status = this->afterRun();
    CGRAPH_FUNCTION_END
}


CSTATUS GCondition::run() {
    CGRAPH_FUNCTION_BEGIN

    int loop = 0;
    int index = this->choose();
    if (GROUP_LAST_ELEMENT_INDEX == index && this->condition_elements_.size() > 0) {
        // 如果返回-1，则直接执行最后一个条件（模仿default功能）
        loop = condition_elements_.back()->loop_;
        while (loop--) {
            status = condition_elements_.back()->run();
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    } else if (0 <= index && index < condition_elements_.size()) {
        // 如果返回的内容，在元素范围之内，则直接执行元素的内容
        loop = condition_elements_[index]->loop_;
        while (loop--) {
            status = condition_elements_[index]->run();
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    } else {
        // 设定的index不在范围内，则不进行任何操作
    }

    CGRAPH_FUNCTION_END
}


int GCondition::getRange() const {
    return this->condition_elements_.size();
}
