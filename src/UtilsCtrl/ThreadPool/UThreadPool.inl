/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPool.inl
@Time: 2021/7/4 1:34 下午
@Desc:
***************************/

#ifndef CGRAPH_UTHREADPOOL_INL
#define CGRAPH_UTHREADPOOL_INL

#include "UThreadPool.h"

CGRAPH_NAMESPACE_BEGIN

template<typename FunctionType>
auto UThreadPool::commit(const FunctionType& func, CIndex index)
-> std::future<decltype(std::declval<FunctionType>()())> {
    using ResultType = decltype(std::declval<FunctionType>()());

    std::packaged_task<ResultType()> task(func);
    std::future<ResultType> result(task.get_future());

    execute(std::move(task), index);
    return result;
}


template<typename FunctionType>
auto UThreadPool::commitWithTid(const FunctionType& func, CIndex tid, CBool enable, CBool lockable)
-> std::future<decltype(std::declval<FunctionType>()())> {
    using ResultType = decltype(std::declval<FunctionType>()());
    std::packaged_task<ResultType()> task(std::move(func));
    std::future<ResultType> result(task.get_future());

    execute(std::move(task), tid, enable, lockable);
    return result;
}


template<typename FunctionType>
auto UThreadPool::commitWithPriority(const FunctionType& func, int priority)
-> std::future<decltype(std::declval<FunctionType>()())> {
    using ResultType = decltype(std::declval<FunctionType>()());

    std::packaged_task<ResultType()> task(func);
    std::future<ResultType> result(task.get_future());

    if (secondary_threads_.empty()) {
        createSecondaryThread(1);    // 如果没有开启辅助线程，则直接开启一个
    }

    priority_task_queue_.push(std::move(task), priority);
    return result;
}


template<typename FunctionType>
CVoid UThreadPool::execute(FunctionType&& task, CIndex index) {
    CIndex realIndex = dispatch(index);
    if (realIndex >= 0 && realIndex < config_.default_thread_size_) {
        primary_threads_[realIndex]->pushTask(std::forward<FunctionType>(task));
    } else if (CGRAPH_LONG_TIME_TASK_STRATEGY == realIndex) {
        priority_task_queue_.push(std::forward<FunctionType>(task), CGRAPH_LONG_TIME_TASK_STRATEGY);
    } else {
        task_queue_.push(std::forward<FunctionType>(task));
    }
}


template<typename FunctionType>
CVoid UThreadPool::executeWithTid(FunctionType&& task, CIndex tid, CBool enable, CBool lockable) {
    if (likely(tid >= 0 && tid < config_.default_thread_size_)) {
        primary_threads_[tid]->pushTask(std::forward<FunctionType>(task), enable, lockable);
    } else {
        // 如果超出主线程的范围，则默认写入 pool 通用的任务队列中
        task_queue_.push(std::forward<FunctionType>(task));
    }
}

CGRAPH_NAMESPACE_END

#endif    // CGRAPH_UTHREADPOOL_INL
