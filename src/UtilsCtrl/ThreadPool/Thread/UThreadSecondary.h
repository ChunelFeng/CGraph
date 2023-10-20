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

CGRAPH_NAMESPACE_BEGIN

class UThreadSecondary : public UThreadBase {
public:
    explicit UThreadSecondary() {
        cur_ttl_ = 0;
        type_ = CGRAPH_THREAD_TYPE_SECONDARY;
    }


protected:
    CStatus init() override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(false)
        CGRAPH_ASSERT_NOT_NULL(config_)

        cur_ttl_ = config_->secondary_thread_ttl_;
        is_init_ = true;
        thread_ = std::move(std::thread(&UThreadSecondary::run, this));
        setSchedParam();
        CGRAPH_FUNCTION_END
    }


    /**
     * 设置pool的信息
     * @param poolTaskQueue
     * @param poolPriorityTaskQueue
     * @param config
     * @return
     */
    CStatus setThreadPoolInfo(UAtomicQueue<UTask>* poolTaskQueue,
                              UAtomicPriorityQueue<UTask>* poolPriorityTaskQueue,
                              UThreadPoolConfigPtr config) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(false)    // 初始化之前，设置参数
        CGRAPH_ASSERT_NOT_NULL(poolTaskQueue, poolPriorityTaskQueue, config)

        this->pool_task_queue_ = poolTaskQueue;
        this->pool_priority_task_queue_ = poolPriorityTaskQueue;
        this->config_ = config;
        CGRAPH_FUNCTION_END
    }


    CStatus run() final {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(true)

        status = loopProcess();
        CGRAPH_FUNCTION_END
    }


    CVoid processTask() override {
        UTask task;
        if (popPoolTask(task)) {
            runTask(task);
        } else {
            // 如果单词无法获取，则稍加等待
            waitRunTask(config_->queue_emtpy_interval_);
        }
    }


    CVoid processTasks() override {
        UTaskArr tasks;
        if (popPoolTask(tasks)) {
            runTasks(tasks);
        } else {
            waitRunTask(config_->queue_emtpy_interval_);
        }
    }


    /**
     * 有等待的执行任务
     * @param ms
     * @return
     * @notice 目的是降低cpu的占用率
     */
    CVoid waitRunTask(CMSec ms) {
        auto task = this->pool_task_queue_->popWithTimeout(ms);
        if (nullptr != task) {
            (*task)();
        }
    }


    /**
     * 判断本线程是否需要被自动释放
     * @return
     */
    bool freeze() {
        if (likely(is_running_)) {
            cur_ttl_++;
            cur_ttl_ = std::min(cur_ttl_, config_->secondary_thread_ttl_);
        } else {
            cur_ttl_--;    // 如果当前线程没有在执行，则ttl-1
        }

        return cur_ttl_ <= 0 && done_;    // 必须是正在执行的线程，才可以被回收
    }

private:
    int cur_ttl_ = 0;                                                      // 当前最大生存周期

    friend class UThreadPool;
};

using UThreadSecondaryPtr = UThreadSecondary *;

CGRAPH_NAMESPACE_END

#endif // CGRAPH_UTHREADSECONDARY_H
