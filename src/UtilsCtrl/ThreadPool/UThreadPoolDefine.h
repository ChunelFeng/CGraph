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

using CGRAPH_READ_LOCK = std::shared_lock<std::shared_mutex>;
using CGRAPH_WRITE_LOCK = std::unique_lock<std::shared_mutex>;
using CGRAPH_LOCK_GUARD = std::lock_guard<std::mutex>;
using CGRAPH_UNIQUE_LOCK = std::unique_lock<std::mutex>;

static const int CGRAPH_DEFAULT_THREAD_SIZE = std::thread::hardware_concurrency();
static const int CGRAPH_MAX_THREAD_SIZE = CGRAPH_DEFAULT_THREAD_SIZE * 2 + 1;

static const int CGRAPH_SECONDARY_THREAD_TTL = 20;
static const int CGRAPH_MONITOR_SPAN = 5;

#endif // CGRAPH_UTHREADPOOLDEFINE_H
