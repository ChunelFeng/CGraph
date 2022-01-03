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
#include "../WorkStealingQueue/UWorkStealingQueue.h"
#include "../AtomicQueue/UAtomicQueue.h"

CGRAPH_NAMESPACE_BEGIN

class UThreadBase : public UThreadObject {

protected:
    explicit UThreadBase() {
        done_ = true;
        is_init_ = false;
        is_running_ = false;
        pool_task_queue_ = nullptr;
        config_ = nullptr;
    }


    ~UThreadBase() override {
        stop();
    }


    /**
     * 所有线程类的deinit函数应该是一样的
     * 但是init函数不一样，因为线程构造函数不同
     * @return
     */
    CStatus deinit() override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(true)

        stop();
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
     * 外部传入具体配置信息
     * @param config
     * @return
     */
    CStatus setConfig(UThreadPoolConfigPtr config) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(false)

        this->config_ = config;
        CGRAPH_FUNCTION_END
    }


protected:
    void stop() {
        done_ = false;
        if (thread_.joinable()) {
            thread_.join();    // 等待线程结束
        }
        is_init_ = false;
        is_running_ = false;
    }

protected:
    bool done_;                                               // 线程状态标记
    bool is_init_;                                            // 标记初始化状态
    bool is_running_;                                         // 是否正在执行

    UAtomicQueue<UTaskWrapper>* pool_task_queue_;             // 用于存放线程池中的普通任务
    UThreadPoolConfigPtr config_;                             // 配置参数信息

    std::thread thread_;                                      // 线程类
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTHREADBASE_H
