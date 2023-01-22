/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEvent.cpp
@Time: 2023/1/20 23:07
@Desc: 
***************************/

#include "GEvent.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GEvent::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(this->thread_pool_)

    // 触发一个事件，放到辅助线程中去，异步执行
    thread_pool_->commit([this] {
        this->trigger(this->param_);
    }, CGRAPH_EVENT_TASK_STRATEGY);
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END