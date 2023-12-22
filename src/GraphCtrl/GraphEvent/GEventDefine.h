/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEventDefine.h
@Time: 2023/3/2 00:19
@Desc: 
***************************/

#ifndef CGRAPH_GEVENTDEFINE_H
#define CGRAPH_GEVENTDEFINE_H

#include "../GraphDefine.h"

CGRAPH_NAMESPACE_BEGIN
CGRAPH_INTERNAL_NAMESPACE_BEGIN

/**
 * 执行模式
 */
enum class GEVENT_TYPE {
    SYNC = 0,        // 同步模式
    ASYNC = 1,       // 异步模式
};


/**
 * 等待异步执行的策略
 */
enum class GEVENT_ASYNC_STRATEGY {
    PIPELINE_RUN_FINISH = 0,              // 在每次pipeline run结束的时候（默认）
    PIPELINE_DESTROY = 1,                 // 每次pipeline 结束的时候
    NO_WAIT = 2,                          // 不等待
};

CGRAPH_INTERNAL_NAMESPACE_END
CGRAPH_NAMESPACE_END

// 方便外部引用
using GEventType = CGraph::internal::GEVENT_TYPE;
using GEventAsyncStrategy = CGraph::internal::GEVENT_ASYNC_STRATEGY;

#endif //CGRAPH_GEVENTDEFINE_H
