/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: ThreadPoolDefine.h
@Time: 2021/7/3 12:24 上午
@Desc: 
***************************/

#ifndef CGRAPH_UTHREADPOOLDEFINE_H
#define CGRAPH_UTHREADPOOLDEFINE_H

#include <thread>
#include <shared_mutex>
#include <memory>
#include <functional>

CGRAPH_NAMESPACE_BEGIN

using CGRAPH_READ_LOCK = std::shared_lock<std::shared_mutex>;
using CGRAPH_WRITE_LOCK = std::unique_lock<std::shared_mutex>;
using CGRAPH_LOCK_GUARD = std::lock_guard<std::mutex>;
using CGRAPH_UNIQUE_LOCK = std::unique_lock<std::mutex>;

using CGRAPH_DEFAULT_FUNCTION = std::function<void()>;             // 默认无参函数类型
using CGRAPH_TASKGROUP_CALLBACK = std::function<void(CStatus)>;    // 任务组执行回调函数

static const int CGRAPH_DEFAULT_THREAD_SIZE = 4;             // 默认主线程个数
static const int CGRAPH_MAX_THREAD_SIZE = 8;                 // 最大线程个数
static const int CGRAPH_MAX_TASK_STEAL_RANGE = 2;            // 盗取机制相邻范围
static const bool CGRAPH_BATCH_TASK_ENABLE = false;          // 是否开启批量任务功能
static const int CGRAPH_MAX_LOCAL_BATCH_SIZE = 2;            // 批量执行本地任务最大值
static const int CGRAPH_MAX_POOL_BATCH_SIZE = 2;             // 批量执行通用任务最大值
static const int CGRAPH_MAX_STEAL_BATCH_SIZE = 2;            // 批量盗取任务最大值
static const bool CGRAPH_FAIR_LOCK_ENABLE = false;           // 是否开启公平锁（非必须场景不建议开启，开启后CGRAPH_BATCH_TASK_ENABLE无效）
static const int CGRAPH_SECONDARY_THREAD_TTL = 10;           // 辅助线程ttl，单位为s
static const int CGRAPH_MONITOR_SPAN = 5;                    // 监控线程执行间隔，单位为s

CGRAPH_NAMESPACE_END

#endif // CGRAPH_UTHREADPOOLDEFINE_H
