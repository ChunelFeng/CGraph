/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GGroup.cpp
@Time: 2021/6/15 1:49 下午
@Desc: 
***************************/

#include "GGroup.h"


CSTATUS GGroup::beforeRun() {
    CGRAPH_FUNCTION_BEGIN
    this->done_ = false;
    this->left_depend_ = (int)dependence_.size();

    CGRAPH_FUNCTION_END
}


CSTATUS GGroup::afterRun() {
    CGRAPH_FUNCTION_BEGIN

    for (auto& element : this->run_before_) {
        element->left_depend_--;
    }
    this->done_ = true;

    CGRAPH_FUNCTION_END
}

