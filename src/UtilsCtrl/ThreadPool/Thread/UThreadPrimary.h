/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPrimary.h
@Time: 2021/7/8 11:02 下午
@Desc: 核心线程，处理任务中
***************************/

#ifndef CGRAPH_UTHREADPRIMARY_H
#define CGRAPH_UTHREADPRIMARY_H

#include <vector>
#include <mutex>

#include "UThreadBase.h"

CGRAPH_NAMESPACE_BEGIN

class UThreadPrimary : public UThreadBase {
protected:
    explicit UThreadPrimary() {
        index_ = CGRAPH_SECONDARY_THREAD_COMMON_ID;
        pool_threads_ = nullptr;
        type_ = CGRAPH_THREAD_TYPE_PRIMARY;
    }


    CStatus init() override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(false)
        CGRAPH_ASSERT_NOT_NULL(config_)

        is_init_ = true;
        buildStealTargets();
        thread_ = std::thread(&UThreadPrimary::run, this);
        setSchedParam();
        setAffinity(index_);
        CGRAPH_FUNCTION_END
    }


    /**
     * 注册线程池相关内容，需要在init之前使用
     * @param index
     * @param poolTaskQueue
     * @param poolThreads
     * @param config
     */
    CStatus setThreadPoolInfo(const int index,
                              UAtomicQueue<UTask>* poolTaskQueue,
                              std::vector<UThreadPrimary *>* poolThreads,
                              const UThreadPoolConfigPtr config) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(false)    // 初始化之前，设置参数
        CGRAPH_ASSERT_NOT_NULL(poolTaskQueue, poolThreads, config)

        this->index_ = index;
        this->pool_task_queue_ = poolTaskQueue;
        this->pool_threads_ = poolThreads;
        this->config_ = config;
        CGRAPH_FUNCTION_END
    }


    /**
     * 线程执行函数
     * @return
     */
    CStatus run() final {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(true)
        CGRAPH_ASSERT_NOT_NULL(pool_threads_)

        /**
         * 线程池中任何一个primary线程为null都不可以执行
         * 防止线程初始化失败的情况，导致的崩溃
         * 理论不会走到这个判断逻辑里面
         */
        if (std::any_of(pool_threads_->begin(), pool_threads_->end(),
                        [](const UThreadPrimary* thd) {
                            return nullptr == thd;
                        })) {
            CGRAPH_RETURN_ERROR_STATUS("primary thread is null")
        }

        loopProcess();
        CGRAPH_FUNCTION_END
    }


    CVoid processTask() override {
        UTask task;
        if (popTask(task) || stealTask(task) || popPoolTask(task)) {
            runTask(task);
        } else {
            fatWait();
        }
    }


    CVoid processTasks() override {
        UTaskArr tasks;
        if (popTask(tasks) || stealTask(tasks) || popPoolTask(tasks)) {
            // 尝试从主线程中获取/盗取批量task，如果成功，则依次执行
            runTasks(tasks);
        } else {
            fatWait();
        }
    }


    /**
     * 如果总是进入无task的状态，则开始休眠
     * 休眠一定时间后，然后恢复执行状态，避免出现异常情况导致无法唤醒
     */
    CVoid fatWait() {
        ++cur_empty_epoch_;
        CGRAPH_YIELD();
        if (cur_empty_epoch_ >= config_->primary_thread_busy_epoch_) {
            CGRAPH_UNIQUE_LOCK lk(mutex_);
            cv_.wait_for(lk, std::chrono::milliseconds(config_->primary_thread_empty_interval_),
                         [this] { return 0 == cur_empty_epoch_ || !wsq_.isEmpty() || !done_; });
            cur_empty_epoch_ = 0;
        }
    }


    /**
     * 依次push到队列里。如果失败，则yield，然后重新push
     * @param task
     * @return
     */
    CVoid pushTask(UTask&& task) {
        wsq_.push(std::move(task));
        {
            CGRAPH_LOCK_GUARD lk(mutex_);
            cur_empty_epoch_ = 0;
        }
        cv_.notify_one();
    }


    /**
     * 写入 task信息，是否上锁由
     * @param task
     * @param enable 确认是否有
     * @param lockable true 的时候需要上锁，false 的时候会解锁
     * @return
     */
    CVoid pushTask(UTask&& task, const CBool enable, const CBool lockable) {
        wsq_.push(std::move(task), enable, lockable);
        if (enable && !lockable) {
            {
                CGRAPH_LOCK_GUARD lk(mutex_);
                cur_empty_epoch_ = 0;
            }
            cv_.notify_one();
        }
    }


    /**
     * 从本地弹出一个任务
     * @param task
     * @return
     */
    CBool popTask(UTaskRef task) {
        return wsq_.tryPop(task);
    }


    /**
     * 从本地弹出一批任务
     * @param tasks
     * @return
     */
    CBool popTask(UTaskArrRef tasks) {
        return wsq_.tryPop(tasks, config_->max_local_batch_size_);
    }


    /**
     * 从其他线程窃取一个任务
     * @param task
     * @return
     */
    CBool stealTask(UTaskRef task) const {
        if (unlikely(pool_threads_->size() < static_cast<CSize>(config_->default_thread_size_))) {
            /**
             * 线程池还未初始化完毕的时候，无法进行steal。
             * 确保程序安全运行。
             */
            return false;
        }

        CBool result = false;
        for (const auto target : steal_targets_) {
            /**
            * 从线程中周围的thread中，窃取任务。
            * 如果成功，则返回true，并且执行任务。
            */
            if (likely((*pool_threads_)[target])
                && ((*pool_threads_)[target])->wsq_.trySteal(task)) {
                result = true;
                break;
            }
        }

        return result;
    }


    /**
     * 从其他线程盗取一批任务
     * @param tasks
     * @return
     */
    CBool stealTask(UTaskArrRef tasks) const {
        if (unlikely(pool_threads_->size() != static_cast<CSize>(config_->default_thread_size_))) {
            return false;
        }

        CBool result = false;
        for (const auto target : steal_targets_) {
            if (likely((*pool_threads_)[target])) {
                result = ((*pool_threads_)[target])->wsq_.trySteal(tasks, config_->max_steal_batch_size_);
                if (result) {
                    /**
                     * 在这里，我们对模型进行了简化。实现的思路是：
                     * 尝试从邻居主线程中，获取 x(=max_steal_batch_size_) 个task，
                     * 如果从某一个邻居中，获取了 y(<=x) 个task，则也终止steal的流程
                     * 且如果如果有一次批量steal成功，就认定成功
                     */
                    break;
                }
            }
        }

        return result;
    }


    /**
     * 构造 steal 范围的 target，避免每次盗取的时候，重复计算
     * @return
     */
    CVoid buildStealTargets() {
        steal_targets_.clear();
        for (int i = 0; i < config_->calcStealRange(); i++) {
            auto target = (index_ + i + 1) % config_->default_thread_size_;
            steal_targets_.push_back(target);
        }
        steal_targets_.shrink_to_fit();
    }

private:
    CInt index_ {0};                                                  // 线程index
    std::atomic<CInt> cur_empty_epoch_ {0 };                       // 当前空转的轮数信息
    UWorkStealingQueue<UTask> wsq_ {};                                // 内部队列信息
    std::vector<UThreadPrimary *>* pool_threads_ {};                  // 用于存放线程池中的线程信息
    std::vector<CInt> steal_targets_ {};                              // 被偷的目标信息

    friend class UThreadPool;
    friend class CAllocator;
};

using UThreadPrimaryPtr = UThreadPrimary *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTHREADPRIMARY_H
