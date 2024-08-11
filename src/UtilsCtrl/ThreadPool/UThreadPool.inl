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
auto UThreadPool::commit(const FunctionType& task, CIndex index)
-> std::future<decltype(std::declval<FunctionType>()())> {
    using ResultType = decltype(std::declval<FunctionType>()());

    std::packaged_task<ResultType()> runnableTask(std::move(task));
    std::future<ResultType> result(runnableTask.get_future());

    CIndex realIndex = dispatch(index);
    if (realIndex >= 0 && realIndex < config_.default_thread_size_) {
        // 如果返回的结果，在主线程数量之间，则放到主线程的queue中执行
        primary_threads_[realIndex]->pushTask(std::move(runnableTask));
    } else if (CGRAPH_LONG_TIME_TASK_STRATEGY == realIndex) {
        /**
         * 如果是长时间任务，则交给特定的任务队列，仅由辅助线程处理
         * 目的是防止有很多长时间任务，将所有运行的线程均阻塞
         * 长任务程序，默认优先级较低
         **/
        priority_task_queue_.push(std::move(runnableTask), CGRAPH_LONG_TIME_TASK_STRATEGY);
    } else {
        // 返回其他结果，放到pool的queue中执行
        task_queue_.push(std::move(runnableTask));
    }
    return result;
}


template<typename FunctionType>
auto UThreadPool::commitWithTid(const FunctionType& task, CIndex tid, CBool enable, CBool lockable)
-> std::future<decltype(std::declval<FunctionType>()())> {
    using ResultType = decltype(std::declval<FunctionType>()());
    std::packaged_task<ResultType()> runnableTask(std::move(task));
    std::future<ResultType> result(runnableTask.get_future());

    if (tid >= 0 && tid < config_.default_thread_size_) {
        primary_threads_[tid]->pushTask(std::move(runnableTask), enable, lockable);
    } else {
        // 如果超出主线程的范围，则默认写入 pool 通用的任务队列中
        task_queue_.push(task);
    }
    return result;
}


template<typename FunctionType>
auto UThreadPool::commitWithPriority(const FunctionType& task, int priority)
-> std::future<decltype(std::declval<FunctionType>()())> {
    using ResultType = decltype(std::declval<FunctionType>()());

    std::packaged_task<ResultType()> runnableTask(task);
    std::future<ResultType> result(runnableTask.get_future());

    if (secondary_threads_.empty()) {
        createSecondaryThread(1);    // 如果没有开启辅助线程，则直接开启一个
    }

    priority_task_queue_.push(std::move(runnableTask), priority);
    return result;
}


template<typename FunctionType>
CVoid UThreadPool::execute(const FunctionType& task, CIndex index) {
    CIndex realIndex = dispatch(index);
    if (realIndex >= 0 && realIndex < config_.default_thread_size_) {
        primary_threads_[realIndex]->pushTask(std::move(task));
    } else if (CGRAPH_LONG_TIME_TASK_STRATEGY == realIndex) {
        priority_task_queue_.push(std::move(task), CGRAPH_LONG_TIME_TASK_STRATEGY);
    } else {
        task_queue_.push(std::move(task));
    }
}

CGRAPH_NAMESPACE_END

#endif    // CGRAPH_UTHREADPOOL_INL
