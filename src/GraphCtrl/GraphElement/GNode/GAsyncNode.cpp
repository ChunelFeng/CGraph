/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAsyncNode.cpp
@Time: 2023/8/10 22:00
@Desc: 
***************************/

#include "GAsyncNode.h"

CGRAPH_NAMESPACE_BEGIN

GAsyncNode::GAsyncNode() {
    element_type_ = GElementType::ASYNC_NODE;
    session_ = URandom<>::generateSession(CGRAPH_STR_ASYNC_NODE);
}


CStatus GAsyncNode::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    if (loop_ > 1) {
        CGRAPH_RETURN_ERROR_STATUS("GAsyncNode can set loop=1 only")
    }

    if (timeout_ != 0) {
        CGRAPH_RETURN_ERROR_STATUS("GAsyncNode can set timeout=0 only")
    }

    async_result_ = this->thread_pool_->commitWithPriority([this] {
        return this->asyncRun();
    }, CGRAPH_ASYNC_NODE_TASK_STRATEGY);

    CGRAPH_FUNCTION_END
}


CBool GAsyncNode::isAsync() const {
    return true;
}

CGRAPH_NAMESPACE_END