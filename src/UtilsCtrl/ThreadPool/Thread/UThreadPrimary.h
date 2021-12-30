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

CGRAPH_NAMESPACE_BEGIN

class UThreadPrimary : public UThreadBase {
public:
    explicit UThreadPrimary() {
        index_ = -1;
        pool_threads_ = nullptr;
    }

    CStatus init() override {
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
    CStatus setThreadPoolInfo(int index,
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
    CStatus run() override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(true)
        CGRAPH_ASSERT_NOT_NULL(pool_threads_)

        /**
         * 线程池中任何一个primary线程为null都不可以执行
         * 防止线程初始化失败的情况，导致的崩溃
         * 理论不会走到这个判断逻辑里面
         */
        if (std::any_of(pool_threads_->begin(), pool_threads_->end(),
                        [](UThreadPrimary* thd) {
                            return nullptr == thd;
                        })) {
            return CStatus(STATUS_ERR, "thread is null");
        }

        if (REAL_BATCH_TASKS_RATIO) {
            while (done_) {
                runTasks();    // 批量任务获取执行接口
            }
        } else {
            while (done_) {
                runTask();    // 单个任务获取执行接口
            }
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
     * 批量执行task信息
     */
    void runTasks() {
        UTaskWrapperArr tasks;
        if (popTask(tasks) || popPoolTask(tasks) || stealTask(tasks)) {
            // 尝试从主线程中获取/盗取批量task，如果成功，则依次执行
            is_running_ = true;
            for (auto& curTask : tasks) {
                curTask();
            }
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
    bool popTask(UTaskWrapperRef task) {
        return work_stealing_queue_.tryPop(task);
    }


    /**
     * 从本地弹出一批任务
     * @param tasks
     * @return
     */
    bool popTask(UTaskWrapperArrRef tasks) {
        return work_stealing_queue_.tryPop(tasks);
    }


    /**
     * 从其他线程窃取一个任务
     * @param task
     * @return
     */
    bool stealTask(UTaskWrapperRef task) {
        if (unlikely(pool_threads_->size() < CGRAPH_DEFAULT_THREAD_SIZE)) {
            /**
             * 线程池还未初始化完毕的时候，无法进行steal。
             * 确保程序安全运行。
             */
            return false;
        }

        /**
         * 窃取的时候，仅从相邻的primary线程中窃取
         * 待窃取相邻的数量，不能超过默认primary线程数
         */
        int range = std::min(CGRAPH_MAX_TASK_STEAL_RANGE, CGRAPH_DEFAULT_THREAD_SIZE - 1);
        for (int i = 0; i < range; i++) {
            /**
            * 从线程中周围的thread中，窃取任务。
            * 如果成功，则返回true，并且执行任务。
            */
            int curIndex = (index_ + i + 1) % CGRAPH_DEFAULT_THREAD_SIZE;
            if (nullptr != (*pool_threads_)[curIndex]
                && ((*pool_threads_)[curIndex])->work_stealing_queue_.trySteal(task)) {
                return true;
            }
        }

        return false;
    }


    /**
     * 从其他线程盗取一批任务
     * @param tasks
     * @return
     */
    bool stealTask(UTaskWrapperArrRef tasks) {
        if (unlikely(pool_threads_->size() < CGRAPH_DEFAULT_THREAD_SIZE)) {
            return false;
        }

        int range = std::min(CGRAPH_MAX_TASK_STEAL_RANGE, CGRAPH_DEFAULT_THREAD_SIZE - 1);
        for (int i = 0; i < range; i++) {
            int curIndex = (index_ + i + 1) % CGRAPH_DEFAULT_THREAD_SIZE;
            if (nullptr != (*pool_threads_)[curIndex]
                && ((*pool_threads_)[curIndex])->work_stealing_queue_.trySteal(tasks)) {
                return true;
            }
        }

        return false;
    }

private:
    int index_ {-1};                                               // 线程index
    UWorkStealingQueue work_stealing_queue_;                       // 内部队列信息

    std::vector<UThreadPrimary *>* pool_threads_ { nullptr };      // 用于存放线程池中的线程信息

    friend class UThreadPool;
};

using UThreadPrimaryPtr = UThreadPrimary *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTHREADPRIMARY_H
