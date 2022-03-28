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

    UThreadPoolPtr tp = UThreadPoolSingleton::get(false);
    CGRAPH_ASSERT_NOT_NULL(tp)

    status = tp->submit(tasks, ttl);
    CGRAPH_FUNCTION_END
}


CSize GNode::getThreadId() {
    const CSize& tid = (CSize) std::hash<std::thread::id>{}(std::this_thread::get_id());
    return tid;
}

CGRAPH_NAMESPACE_END