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
auto UThreadPool::commit(FunctionType&& func, CIndex index)
-> std::future<decltype(std::declval<typename std::decay<FunctionType>::type>()())> {
    using ResultType = decltype(std::declval<typename std::decay<FunctionType>::type>()());

    std::packaged_task<ResultType()> task(std::forward<FunctionType>(func));
    std::future<ResultType> result(task.get_future());

    execute(std::move(task), index);
    return result;
}


template<typename FunctionType>
auto UThreadPool::commitWithTid(FunctionType&& func, CIndex tid, CBool enable, CBool lockable)
-> std::future<decltype(std::declval<typename std::decay<FunctionType>::type>()())> {
    using ResultType = decltype(std::declval<typename std::decay<FunctionType>::type>()());
    std::packaged_task<ResultType()> task(std::forward<FunctionType>(func));
    std::future<ResultType> result(task.get_future());

    executeWithTid(std::move(task), tid, enable, lockable);
    return result;
}


template<typename FunctionType,
    typename std::enable_if<!std::is_same<typename std::decay<FunctionType>::type, UTask>::value, int>::type>
CVoid UThreadPool::execute(FunctionType&& task, const CIndex index) {
    envokeTask(UTask(std::forward<FunctionType>(task)), index);
}


template<typename FunctionType>
CVoid UThreadPool::executeWithTid(FunctionType&& task, const CIndex tid, const CBool enable, const CBool lockable) {
    if (likely(tid >= 0 && tid < config_.default_thread_size_)) {
        primary_threads_[tid]->pushTask(UTask(std::forward<FunctionType>(task)), enable, lockable);
    } else {
        // 如果超出主线程的范围，则默认写入 pool 通用的任务队列中
        task_queue_.push(UTask(std::forward<FunctionType>(task)));
    }
}

CGRAPH_NAMESPACE_END

#endif    // CGRAPH_UTHREADPOOL_INL
