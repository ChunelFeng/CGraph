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

    this->condition_elements_.push_back(element);
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

    int index = this->choose();
    if (GROUP_LAST_ELEMENT_INDEX == index && this->condition_elements_.size() > 0) {
        // 如果返回-1，则直接执行最后一个条件（模仿default功能）
        status = (*this->condition_elements_.end())->run();
    } else if (index >= 0 && index < condition_elements_.size()) {
        // 如果返回的内容，在元素范围之内，则直接执行元素的内容
        status = condition_elements_[index]->run();
    } else {
        // 设定的index不在范围内，返回错误信息
        status = STATUS_ERR;
    }

    CGRAPH_FUNCTION_END
}
