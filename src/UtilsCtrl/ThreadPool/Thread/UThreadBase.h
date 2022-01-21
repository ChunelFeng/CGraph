/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadBase.h
@Time: 2021/7/2 11:24 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTHREADBASE_H
#define CGRAPH_UTHREADBASE_H

#include <thread>

#include "../UThreadObject.h"
#include "../Queue/UQueueInclude.h"


CGRAPH_NAMESPACE_BEGIN

class UThreadBase : public UThreadObject {

protected:
    explicit UThreadBase() {
        done_ = true;
        is_init_ = false;
        is_running_ = false;
        pool_task_queue_ = nullptr;
        config_ = nullptr;
        total_task_num_ = 0;
    }


    ~UThreadBase() override {
        reset();
    }


    /**
     * 所有线程类的deinit函数应该是一样的
     * 但是init函数不一样，因为线程构造函数不同
     * @return
     */
    CStatus deinit() override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(true)

        reset();
        CGRAPH_FUNCTION_END
    }


    /**
     * 从线程池的队列中，获取任务
     * @param task
     * @return
     */
    virtual bool popPoolTask(UTaskWrapperRef task) {
        return (pool_task_queue_ && pool_task_queue_->tryPop(task));
    }


    /**
     * 从线程池的队列中中，获取批量任务
     * @param tasks
     * @return
     */
    virtual bool popPoolTask(UTaskWrapperArrRef tasks) {
        return (pool_task_queue_ && pool_task_queue_->tryPop(tasks, config_->max_pool_batch_size_));
    }


    /**
     * 执行单个任务
     * @param task
     */
    void runTask(UTaskWrapper& task) {
        is_running_ = true;
        task();
        total_task_num_++;
        is_running_ = false;
    }


    /**
     * 批量执行任务
     * @param tasks
     */
    void runTasks(UTaskWrapperArr& tasks) {
        is_running_ = true;
        for (auto& task : tasks) {
            task();
        }
        total_task_num_ += tasks.size();
        is_running_ = false;
    }


    /**
     * 清空所有任荣
     */
    void reset() {
        done_ = false;
        if (thread_.joinable()) {
            thread_.join();    // 等待线程结束
        }
        is_init_ = false;
        is_running_ = false;
        total_task_num_ = 0;
    }


    /**
    * 设置线程优先级，仅针对非windows平台使用
    */
    CStatus setSchedParam() {
        CGRAPH_FUNCTION_BEGIN
#ifndef _WIN32
        int priority = CGRAPH_THREAD_SCHED_OTHER;
        int policy = CGRAPH_THREAD_MIN_PRIORITY;
        if (type_ == CGRAPH_THREAD_TYPE_PRIMARY) {
            priority = config_->primary_thread_priority_;
            policy = config_->primary_thread_policy_;
        } else if (type_ == CGRAPH_THREAD_TYPE_SECONDARY) {
            priority = config_->secondary_thread_priority_;
            policy = config_->secondary_thread_policy_;
        }

        auto handle = thread_.native_handle();
        sched_param param = { calcPriority(priority) };
        int ret = pthread_setschedparam(handle, calcPolicy(policy), &param);
        if (0 != ret) {
            return CStatus("set thread param failed, error code is " + std::to_string(ret));
        }
#endif

        CGRAPH_FUNCTION_END
    }


private:
    /**
     * 设定计算线程调度策略信息，
     * 非OTHER/RR/FIFO对应数值，统一返回OTHER类型
     * @param policy
     * @return
     */
    [[nodiscard]] static int calcPolicy(int policy) {
        return (CGRAPH_THREAD_SCHED_OTHER == policy
                || CGRAPH_THREAD_SCHED_RR == policy
                || CGRAPH_THREAD_SCHED_FIFO == policy)
               ? policy : CGRAPH_THREAD_SCHED_OTHER;
    }


    /**
     * 设定线程优先级信息
     * 超过[min,max]范围，统一设置为min值
     * @param priority
     * @return
     */
    [[nodiscard]] static int calcPriority(int priority) {
        return (priority >= CGRAPH_THREAD_MIN_PRIORITY
                && priority <= CGRAPH_THREAD_MAX_PRIORITY)
               ? priority : CGRAPH_THREAD_MIN_PRIORITY;
    }


protected:
    bool done_;                                               // 线程状态标记
    bool is_init_;                                            // 标记初始化状态
    bool is_running_;                                         // 是否正在执行
    int type_ = 0;                                            // 用于区分线程类型（主线程、辅助线程）
    unsigned long long total_task_num_;                       // 处理的任务的数字

    UAtomicQueue<UTaskWrapper>* pool_task_queue_;             // 用于存放线程池中的普通任务
    UThreadPoolConfigPtr config_;                             // 配置参数信息

    std::thread thread_;                                      // 线程类
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTHREADBASE_H
