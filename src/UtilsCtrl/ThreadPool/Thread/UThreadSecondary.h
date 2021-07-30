/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadSecondary.h
@Time: 2021/7/8 11:02 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTHREADSECONDARY_H
#define CGRAPH_UTHREADSECONDARY_H

#include "UThreadBase.h"

class UThreadSecondary : public UThreadBase {
public:
    explicit UThreadSecondary() {
        pool_task_queue_ = nullptr;
        ttl_ = 0;
    }


    CSTATUS init() override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(false)

        ttl_ = CGRAPH_SECONDARY_THREAD_TTL;
        thread_ = std::move(std::thread(&UThreadSecondary::run, this));
        is_init_ = true;
        CGRAPH_FUNCTION_END
    }


    /**
     * 设置pool的信息
     * @param poolTaskQueue
     * @return
     */
    CSTATUS setThreadPoolInfo(UAtomicQueue<UTaskWrapper>* poolTaskQueue) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(false)    // 初始化之前，设置参数
        CGRAPH_ASSERT_NOT_NULL(poolTaskQueue)

        this->pool_task_queue_ = poolTaskQueue;
        CGRAPH_FUNCTION_END
    }


    CSTATUS run() override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(true)

        while (done_) {
            runTask();
        }

        CGRAPH_FUNCTION_END
    }


    /**
     * 从线程池的队列中中，获取信息
     * @param task
     * @return
     */
    bool popPoolTask(UTaskWrapperRef task) {
        return (pool_task_queue_ && pool_task_queue_->tryPop(task));
    }


    /**
     * 任务执行函数，从线程池的任务队列中获取信息
     */
    void runTask() {
        UTaskWrapper task;
        if (popPoolTask(task)) {
            is_running_ = true;
            task();
            is_running_ = false;
        } else {
            std::this_thread::yield();
        }
    }


    /**
     * 判断本线程是否需要被自动释放
     * @return
     */
    bool freeze() {
        if (is_running_) {
            ttl_++;
            ttl_ = std::min(ttl_, CGRAPH_SECONDARY_THREAD_TTL);
        } else {
            ttl_--;    // 如果当前线程没有在执行，则ttl-1
        }

        return ttl_ <= 0 ? true : false;
    }

private:
    int ttl_;                                                             // 最大生存周期
    UAtomicQueue<UTaskWrapper>* pool_task_queue_{};                       // 线程池传递下来的信息

    friend class UThreadPool;
};

using UThreadSecondaryPtr = UThreadSecondary *;


#endif // CGRAPH_UTHREADSECONDARY_H
