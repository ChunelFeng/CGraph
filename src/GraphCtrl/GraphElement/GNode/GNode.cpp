/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNode.cpp
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

#include "GNode.h"

GNode::GNode() : GElement() {
    param_manager_ = nullptr;
}


GNode::~GNode() = default;


GNode::GNode(const GNode& node) = default;


GNode& GNode::operator=(const GNode& node) {
    return *this;
}


CSTATUS GNode::process(bool isMock) {
    CGRAPH_FUNCTION_BEGIN
    status = this->beforeRun();
    CGRAPH_FUNCTION_CHECK_STATUS

    int loop = this->loop_;
    while (!isMock && loop--) {
        status = run();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    status = this->afterRun();
    CGRAPH_FUNCTION_END
}
