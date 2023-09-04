/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEvent.cpp
@Time: 2023/1/20 23:07
@Desc: 
***************************/

#include "GEvent.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GEvent::process(GEventType type) {
    CGRAPH_FUNCTION_BEGIN

    switch (type) {
        case GEventType::SYNC:    // 同步触发
            this->trigger(this->param_);
            break;
        case GEventType::ASYNC:    // 异步触发
            CGRAPH_ASSERT_NOT_NULL(this->thread_pool_)
            thread_pool_->commit([this] {
                this->trigger(this->param_);
            }, CGRAPH_POOL_TASK_STRATEGY);
            break;
        default:
            CGRAPH_RETURN_ERROR_STATUS("unknown event type")
    }

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END