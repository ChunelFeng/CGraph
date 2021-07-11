/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPrimary.h
@Time: 2021/7/8 11:02 下午
@Desc: 核心线程，处理任务中
***************************/

#ifndef CGRAPH_UTHREADPRIMARY_H
#define CGRAPH_UTHREADPRIMARY_H

#include "UThreadBase.h"

class UThreadPrimary : public UThreadBase {
public:
    explicit UThreadPrimary() {
        index_ = -1;
        pool_task_queue_ = nullptr;    // 初始化的时候，需要
        pool_threads_ = nullptr;
    }

    CSTATUS init() override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(false)

        thread_ = std::move(std::thread(&UThreadPrimary::run, this));
        is_init_ = true;
        CGRAPH_FUNCTION_END
    }


    /**
     * 注册线程池相关内容，需要在init之前使用
     * @param poolTaskQueue
     * @param poolThreads
     */
    CSTATUS setThreadPoolInfo(int index,
                              UAtomicQueue<UTaskWrapper>* poolTaskQueue,
                              std::vector<UThreadPrimary *>* poolThreads) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(false)    // 初始化之前，设置参数
        CGRAPH_ASSERT_NOT_NULL(poolTaskQueue)
        CGRAPH_ASSERT_NOT_NULL(poolThreads)

        this->index_ = index;
        this->pool_task_queue_ = poolTaskQueue;
        this->pool_threads_ = poolThreads;
        CGRAPH_FUNCTION_END
    }


    /**
     * 线程执行函数
     * @return
     */
    CSTATUS run() override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(true)

        while (done_) {
            runTask();
        }

        CGRAPH_FUNCTION_END
    }


    /**
     * 执行任务
     * @return
     */
    void runTask() {
        UTaskWrapper task;
        if (popTask(task) || popPoolTask(task) || stealTask(task)) {
            is_running_ = true;
            task();
            is_running_ = false;
        } else {
            std::this_thread::yield();
        }
    }


    /**
     * 从本地弹出一个任务
     * @param task
     * @return
     */
    bool popTask(UFunctionWapperRef task) {
        return work_stealing_queue_.tryPop(task);
    }


    /**
     * 从线程池的队列中中，获取信息
     * @param task
     * @return
     */
    bool popPoolTask(UFunctionWapperRef task) {
        return (pool_task_queue_ && pool_task_queue_->tryPop(task));
    }


    /**
     * 从其他线程窃取一个任务
     * @param task
     * @return
     */
    bool stealTask(UFunctionWapperRef task) {
        if (nullptr == pool_threads_) {
            return false;
        }

        // 窃取的时候，仅从primary线程中窃取
        int size = std::min((int)pool_threads_->size(), CGRAPH_DEFAULT_THREAD_SIZE);
        for (int i = 0; i < (size - 1); i++) {
            /**
             * 从线程中周围的thread中，窃取任务。
             * 如果成功，则返回true，并且执行任务。
             * 重新获取一下size，是考虑到动态扩容可能会影响
             */
            int curIndex = (index_ + i + 1) % size;
            if (nullptr != (*pool_threads_)[curIndex]
                && (((*pool_threads_)[curIndex]))->work_stealing_queue_.trySteal(task)) {
                return true;
            }
        }

        return false;
    }

private:
    int index_ {-1};                                               // 线程index
    UWorkStealingQueue work_stealing_queue_;                       // 内部队列信息

    /* 以下两个信息，用于同步本线程所属的线程池中的信息 */
    UAtomicQueue<UTaskWrapper>* pool_task_queue_{};                // 用于存放线程池中的普通任务
    std::vector<UThreadPrimary *>* pool_threads_{};                // 用于存放线程池中的线程信息

    friend class UThreadPool;
};

using UThreadPrimaryPtr = UThreadPrimary *;

#endif //CGRAPH_UTHREADPRIMARY_H
