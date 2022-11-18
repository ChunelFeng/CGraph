/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPool.cpp
@Time: 2022/10/3 17:31
@Desc: 
***************************/

#include "UThreadPool.h"

CGRAPH_NAMESPACE_BEGIN

UThreadPool::UThreadPool(CBool autoInit, const UThreadPoolConfig& config) noexcept {
    cur_index_ = 0;
    is_init_ = false;
    input_task_num_ = 0;
    this->setConfig(config);    // setConfig 函数，用在 is_init_ 设定之后
    is_monitor_ = config_.monitor_enable_;        /** 根据参数设定，决定是否开启监控线程。默认开启 */
    /**
     * CGraph 本身支持跨平台运行
     * 如果在windows平台上，通过Visual Studio(2017版本或以下) 版本，将 UThreadPool 类封装程.dll文件时，遇到无法启动的问题
     * 请参考此链接：https://github.com/ChunelFeng/CGraph/issues/17
     */
    monitor_thread_ = std::move(std::thread(&UThreadPool::monitor, this));
    if (autoInit) {
        this->init();
    }
}


UThreadPool::~UThreadPool() {
    is_monitor_ = false;    // 在析构的时候，才释放监控线程。先释放监控线程，再释放其他的线程
    if (monitor_thread_.joinable()) {
        monitor_thread_.join();
    }

    destroy();
}


CStatus UThreadPool::setConfig(const UThreadPoolConfig &config) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)    // 初始化后，无法设置参数信息

    this->config_ = config;
    CGRAPH_FUNCTION_END
}


CStatus UThreadPool::init() {
    CGRAPH_FUNCTION_BEGIN
    if (is_init_) {
        CGRAPH_FUNCTION_END
    }

    thread_record_map_.clear();
    primary_threads_.reserve(config_.default_thread_size_);
    for (int i = 0; i < config_.default_thread_size_; i++) {
        auto ptr = CGRAPH_SAFE_MALLOC_COBJECT(UThreadPrimary);    // 创建核心线程数
        ptr->setThreadPoolInfo(i, &task_queue_, &primary_threads_, &config_);
        status += ptr->init();

        // 记录线程和匹配id信息
        thread_record_map_.insert(std::pair<CSize, int>((CSize)std::hash<std::thread::id>{}(ptr->thread_.get_id()), i));
        primary_threads_.emplace_back(ptr);
    }
    CGRAPH_FUNCTION_CHECK_STATUS

    /**
     * 策略更新：
     * 初始化的时候，也可以创建n个辅助线程。目的是为了配合仅使用 pool中 priority_queue 的场景
     * 一般情况下，建议为0。
     */
    status = createSecondaryThread(config_.secondary_thread_size_);
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CStatus UThreadPool::submit(const UTaskGroup& taskGroup, CMSec ttl) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)

    std::vector<std::future<CVoid>> futures;
    for (const auto& task : taskGroup.task_arr_) {
        futures.emplace_back(commit(task));
    }

    // 计算最终运行时间信息
    auto deadline = std::chrono::system_clock::now()
                    + std::chrono::milliseconds(std::min(taskGroup.getTtl(), ttl));

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


CStatus UThreadPool::submit(CGRAPH_DEFAULT_CONST_FUNCTION_REF func, CMSec ttl,
                            CGRAPH_CALLBACK_CONST_FUNCTION_REF onFinished) {
    return submit(UTaskGroup(func, ttl, onFinished));
}


int UThreadPool::getThreadNum(CSize tid) {
    int threadNum = -1;
    auto result = thread_record_map_.find(tid);
    if (result != thread_record_map_.end()) {
        threadNum = result->second;
    }

    return threadNum;
}


CStatus UThreadPool::destroy() {
    CGRAPH_FUNCTION_BEGIN
    if (!is_init_) {
        CGRAPH_FUNCTION_END
    }

    // primary 线程是普通指针，需要delete
    for (auto &pt : primary_threads_) {
        status += pt->destroy();
        CGRAPH_DELETE_PTR(pt)
    }
    CGRAPH_FUNCTION_CHECK_STATUS
    primary_threads_.clear();

    // secondary 线程是智能指针，不需要delete
    for (auto &st : secondary_threads_) {
        status += st->destroy();
    }
    CGRAPH_FUNCTION_CHECK_STATUS
    secondary_threads_.clear();
    thread_record_map_.clear();
    is_init_ = false;

    CGRAPH_FUNCTION_END
}


CIndex UThreadPool::dispatch(CIndex origIndex) {
    if (unlikely(config_.fair_lock_enable_)) {
        return CGRAPH_DEFAULT_TASK_STRATEGY;    // 如果开启fair lock，则全部写入 pool的queue中，依次执行
    }

    CIndex realIndex = 0;
    if (CGRAPH_DEFAULT_TASK_STRATEGY == origIndex) {
        /**
         * 如果是默认策略信息，在[0, default_thread_size_) 之间的，通过 thread 中queue来调度
         * 在[default_thread_size_, max_thread_size_) 之间的，通过 pool 中的queue来调度
         */
        realIndex = cur_index_++;
        if (cur_index_ >= config_.max_thread_size_ || cur_index_ < 0) {
            cur_index_ = 0;
        }
    } else {
        realIndex = origIndex;
    }

    return realIndex;    // 交到上游去判断，走哪个线程
}


CStatus UThreadPool::createSecondaryThread(CInt size) {
    CGRAPH_FUNCTION_BEGIN

    int leftSize = (int)(config_.max_thread_size_ - config_.default_thread_size_ - secondary_threads_.size());
    int realSize = std::min(size, leftSize);    // 使用 realSize 来确保所有的线程数量之和，不会超过设定max值
    for (int i = 0; i < realSize; i++) {
        auto ptr = CGRAPH_MAKE_UNIQUE_COBJECT(UThreadSecondary)
        ptr->setThreadPoolInfo(&task_queue_, &priority_task_queue_, &config_);
        status += ptr->init();
        secondary_threads_.emplace_back(std::move(ptr));
    }

    CGRAPH_FUNCTION_END
}


CVoid UThreadPool::monitor() {
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

        // 如果忙碌或者priority_task_queue_中有任务，则需要添加 secondary线程
        if (busy || !priority_task_queue_.empty()) {
            createSecondaryThread(1);
        }

        // 判断 secondary 线程是否需要退出
        for (auto iter = secondary_threads_.begin(); iter != secondary_threads_.end(); ) {
            (*iter)->freeze() ? secondary_threads_.erase(iter++) : iter++;
        }
    }
}

CGRAPH_NAMESPACE_END