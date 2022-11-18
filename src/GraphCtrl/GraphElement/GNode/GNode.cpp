/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNode.cpp
@Time: 2022/3/28 23:14
@Desc: 
***************************/

#include "GNode.h"

CGRAPH_NAMESPACE_BEGIN

GNode::GNode() : GElement() {
    node_type_ = GNodeType::BASIC;
}


CStatus GNode::doParallel(const UTaskGroup& tasks, CMSec ttl) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)

    status = thread_pool_->submit(tasks, ttl);
    CGRAPH_FUNCTION_END
}


GNode* GNode::setType(const GNodeType& type) {
    node_type_ = type;
    return this;
}

CGRAPH_NAMESPACE_END
