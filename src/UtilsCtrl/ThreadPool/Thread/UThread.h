/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThread.h
@Time: 2021/7/2 11:24 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTHREAD_H
#define CGRAPH_UTHREAD_H

#include <thread>

#include "../UThreadObject.h"
#include "../WorkStealingQueue/UWorkStealingQueue.h"
#include "../AtomicQueue/UAtomicQueue.h"

class UThread : public UThreadObject {

protected:
    explicit UThread() {
        index_ = -1;
        done_ = true;
        thread_ = std::move(std::thread(&UThread::run, this));
    }


    ~UThread() override {
        done_ = false;
        if (thread_.joinable()) {
            thread_.join();    // 等待线程结束
        }
    }


    /**
     * 线程执行函数
     * @return
     */
    CSTATUS run() override {
        while (done_) {
            runTask();
        }

        return STATUS_OK;
    }


    /**
     * 将线程池相关内容注册下来
     * @param poolTaskQueue
     * @param poolThreads
     */
    CSTATUS setThreadPoolInfo(int index,
                              UAtomicQueue<UTaskWrapper>* poolTaskQueue,
                              std::vector<UThread *>* poolThreads) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_NOT_NULL(poolTaskQueue)
        CGRAPH_ASSERT_NOT_NULL(poolThreads)

        this->index_ = index;
        this->pool_task_queue_ = poolTaskQueue;
        this->pool_threads_ = poolThreads;
        CGRAPH_FUNCTION_END
    }


    /**
     * 执行任务
     * @return
     */
    void runTask() {
        UTaskWrapper task;
        if (popTask(task)
            || popPoolTask(task)
            || stealTask(task)) {
            task();
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

        int size = pool_threads_->size();
        for (int i = 0; i < (size-1); i++) {
            /**
             * 从线程中周围的thread中，窃取任务。
             * 如果成功，则返回true，并且执行任务。
             * 重新获取一下size，是考虑到动态扩容可能会影响
             */
            int curIndex = (index_ + i + 1) % size;
            if (curIndex < pool_threads_->size()
                && nullptr != (*pool_threads_)[curIndex]
                && ((*pool_threads_)[curIndex])->work_stealing_queue_.trySteal(task)) {
                return true;
            }
        }

        return false;
    }

private:
    bool done_;                                               // 线程状态标记
    int index_;                                               // 线程index
    std::thread thread_;                                      // 线程类
    UWorkStealingQueue work_stealing_queue_;                  // 内部队列信息

    /* 以下两个信息，用于同步本线程所属的线程池中的信息 */
    UAtomicQueue<UTaskWrapper>* pool_task_queue_;             // 用于存放线程池中的普通任务
    std::vector<UThread *>* pool_threads_;                    // 用于存放线程池中的线程信息

    friend class UThreadPool;
};

using UThreadPtr = UThread *;


#endif //CGRAPH_UTHREAD_H
