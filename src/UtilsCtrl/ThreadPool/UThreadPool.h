/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPool.h
@Time: 2021/7/4 1:34 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTHREADPOOL_H
#define CGRAPH_UTHREADPOOL_H

#include <vector>
#include <list>
#include <future>
#include <thread>
#include <algorithm>
#include <memory>
#include <functional>

#include "UThreadObject.h"
#include "UThreadPoolConfig.h"
#include "Queue/UQueueInclude.h"
#include "Thread/UThreadInclude.h"
#include "Task/UTaskGroup.h"

CGRAPH_NAMESPACE_BEGIN

class UThreadPool : public UThreadObject {

public:
    /**
     * 通过默认设置参数，来创建线程池
     * @param autoInit 是否自动开启线程池功能
     * @param config
     */
    explicit UThreadPool(bool autoInit = true,
                         const UThreadPoolConfig& config = UThreadPoolConfig()) noexcept {
        cur_index_ = 0;
        is_init_ = false;
        this->setConfig(config);
        /* 开启监控线程 */
        is_monitor_ = true;
        monitor_thread_ = std::move(std::thread(&UThreadPool::monitor, this));
        autoInit ? this->init() : CStatus();
    }

    ~UThreadPool() override {
        // 在析构的时候，才释放监控线程
        is_monitor_ = false;
        if (monitor_thread_.joinable()) {
            monitor_thread_.join();
        }

        destroy();
    }

    /**
     * 设置线程池相关配置信息，需要在init()函数调用前，完成设置
     * @param config
     * @return
     * @notice 通过单例类(UThreadPoolSingleton)开启线程池，则线程池默认init。需要 destroy 后才可以设置参数
     */
    CStatus setConfig(const UThreadPoolConfig &config) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(false)    // 初始化后，无法设置参数信息

        this->config_ = config;
        CGRAPH_FUNCTION_END
    }

    /**
     * 开启所有的线程信息
     * @return
     */
    CStatus init() final {
        CGRAPH_FUNCTION_BEGIN
        if (is_init_) {
            CGRAPH_FUNCTION_END
        }

        primary_threads_.reserve(config_.default_thread_size_);
        for (int i = 0; i < config_.default_thread_size_; ++i) {
            auto ptr = CGRAPH_SAFE_MALLOC_COBJECT(UThreadPrimary)    // 创建核心线程数

            ptr->setThreadPoolInfo(i, &task_queue_, &primary_threads_, &config_);
            status = ptr->init();
            CGRAPH_FUNCTION_CHECK_STATUS

            primary_threads_.emplace_back(ptr);
        }

        is_init_ = true;
        CGRAPH_FUNCTION_END
    }

    /**
     * 提交任务信息
     * @tparam FunctionType
     * @param func
     * @param index
     * @return
     */
    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> commit(const FunctionType& func,
                                                                      int index = CGRAPH_DEFAULT_TASK_STRATEGY) {
        typedef typename std::result_of<FunctionType()>::type resultType;

        std::packaged_task<resultType()> task(func);
        std::future<resultType> result(task.get_future());

        /**
         * 将任务分配到对应的线程上去执行
         * 如果传入的是CGRAPH_DEFAULT_TASK_STRATEGY，则均分任务
         */
        dispatchTask(std::move(task), index);
        return result;
    }

    /**
     * 执行任务组信息
     * 取taskGroup内部ttl和入参ttl的最小值，为计算ttl标准
     * @param taskGroup
     * @param ttlMs
     * @return
     */
    CStatus submit(const UTaskGroup& taskGroup,
                   int ttlMs = CGRAPH_MAX_BLOCK_TTL_MS) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(true)

        std::vector<std::future<void>> futures;
        for (const auto& task : taskGroup.task_arr_) {
            futures.emplace_back(commit(task));
        }

        // 计算最终运行时间信息
        std::chrono::system_clock::time_point deadline
                = std::chrono::system_clock::now() + std::chrono::milliseconds(std::min(taskGroup.getTtlMs(), ttlMs));

        for (auto& fut : futures) {
            const auto& futStatus = fut.wait_until(deadline);
            switch (futStatus) {
                case std::future_status::ready: break;    // 正常情况，直接返回了
                case std::future_status::timeout: status += CStatus("thread status timeout"); break;
                case std::future_status::deferred: status += CStatus("thread status deferred"); break;
                default: status += CStatus("thread status unknown");
            }
        }

        if (taskGroup.on_finished_) {
            taskGroup.on_finished_(status);
        }

        CGRAPH_FUNCTION_END
    }

    /**
     * 针对单个任务的情况，复用任务组信息，实现单个任务直接执行
     * @param task
     * @param ttlMs
     * @return
     */
    CStatus submit(CGRAPH_DEFAULT_CONST_FUNCTION_REF func,
                   int ttlMs = CGRAPH_MAX_BLOCK_TTL_MS,
                   CGRAPH_CALLBACK_CONST_FUNCTION_REF onFinished = nullptr) {
        UTaskGroup taskGroup(func, ttlMs, onFinished);
        return submit(taskGroup);
    }

    /**
     * 释放所有的线程信息
     * @return
     */
    CStatus destroy() final {
        CGRAPH_FUNCTION_BEGIN
        if (!is_init_) {
            CGRAPH_FUNCTION_END
        }

        is_init_ = false;
        // primary 线程是普通指针，需要delete
        for (auto& pt : primary_threads_) {
            status = pt->destroy();
            CGRAPH_FUNCTION_CHECK_STATUS
            CGRAPH_DELETE_PTR(pt)
        }
        primary_threads_.clear();

        // secondary 线程是智能指针，不需要delete
        for (auto& st : secondary_threads_) {
            st->destroy();
            CGRAPH_FUNCTION_CHECK_STATUS
        }
        secondary_threads_.clear();

        CGRAPH_FUNCTION_END
    }


protected:
    /**
     * 将任务分配到对应的线程中执行
     * @param task
     * @param index
     */
    void dispatchTask(UTaskWrapper&& task, int index) {
        if (CGRAPH_DEFAULT_TASK_STRATEGY == index) {
            /** 默认调度策略信息 */
            if (!config_.fair_lock_enable_
                && cur_index_ >= 0
                && cur_index_ < config_.default_thread_size_) {
                /** 部分任务直接放到线程的队列中执行 */
                primary_threads_[cur_index_]->work_stealing_queue_.push(std::move(task));
            } else {
                /** 部分数据被分流到线程池（总体）的任务队列中 */
                task_queue_.push(std::move(task));
            }

            cur_index_++;
            if (cur_index_ >= config_.max_thread_size_ || cur_index_ < 0) {
                cur_index_ = 0;
            }
        } else if (index >= 0 && index < config_.default_thread_size_ && !config_.fair_lock_enable_) {
            /** 如果指定的是主线程，则直接放到主线程执行 */
            primary_threads_[index]->work_stealing_queue_.push(std::move(task));
        } else {
            /** 如果是其他情况，则放到通用线程中等待执行 */
            task_queue_.push(std::move(task));
        }
    }

    /**
     * 监控线程执行函数，主要是判断是否需要增加线程，或销毁线程
     * 增/删 操作，仅针对secondary类型线程生效
     */
    void monitor() {
        while (is_monitor_) {
            while (is_monitor_ && !is_init_) {
                // 如果没有init，则一直处于空跑状态
                CGRAPH_SLEEP_SECOND(1)
            }

            int span = config_.monitor_span_;
            while (is_monitor_ && is_init_ && span--) {
                CGRAPH_SLEEP_SECOND(1)    // 保证可以快速退出
            }

            // 如果 primary线程都在执行，则表示忙碌
            bool busy = std::all_of(primary_threads_.begin(), primary_threads_.end(),
                                    [](UThreadPrimaryPtr ptr) { return nullptr != ptr && ptr->is_running_; });

            // 如果忙碌，则需要添加 secondary线程
            if (busy && (secondary_threads_.size() + config_.default_thread_size_) < config_.max_thread_size_) {
                auto ptr = CGRAPH_MAKE_UNIQUE_COBJECT(UThreadSecondary)
                ptr->setThreadPoolInfo(&task_queue_, &config_);
                ptr->init();
                secondary_threads_.emplace_back(std::move(ptr));
            }

            // 判断 secondary 线程是否需要退出
            for (auto iter = secondary_threads_.begin(); iter != secondary_threads_.end(); ) {
                if ((*iter)->freeze()) {
                    secondary_threads_.erase(iter++);
                } else {
                    iter++;
                }
            }
        }
    }


protected:
    bool is_init_ { false };                                                        // 是否初始化
    bool is_monitor_ { true };                                                      // 是否需要监控
    int cur_index_;                                                                 // 记录放入的线程数
    UAtomicQueue<UTaskWrapper> task_queue_;                                         // 用于存放普通任务
    std::vector<UThreadPrimaryPtr> primary_threads_;                                // 记录所有的核心线程
    std::list<std::unique_ptr<UThreadSecondary>> secondary_threads_;                // 用于记录所有的非核心线程数
    UThreadPoolConfig config_;                                                      // 线程池设置值
    std::thread monitor_thread_;                                                    // 监控线程
};

using UThreadPoolPtr = UThreadPool *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTHREADPOOL_H
