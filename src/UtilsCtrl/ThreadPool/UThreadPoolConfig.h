/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPoolConfig.h
@Time: 2022/1/3 9:31 下午
@Desc: 具体值含义，参考UThreadPoolDefine.h文件
***************************/

#ifndef CGRAPH_UTHREADPOOLCONFIG_H
#define CGRAPH_UTHREADPOOLCONFIG_H

#include "UThreadObject.h"
#include "UThreadPoolDefine.h"

CGRAPH_NAMESPACE_BEGIN

struct UThreadPoolConfig : public UThreadObject {
    int default_thread_size_ = CGRAPH_DEFAULT_THREAD_SIZE;
    int max_thread_size_ = CGRAPH_MAX_THREAD_SIZE;
    int max_task_steal_range_ = CGRAPH_MAX_TASK_STEAL_RANGE;
    int max_local_batch_size_ = CGRAPH_MAX_LOCAL_BATCH_SIZE;
    int max_pool_batch_size_ = CGRAPH_MAX_POOL_BATCH_SIZE;
    int max_steal_batch_size_ = CGRAPH_MAX_STEAL_BATCH_SIZE;
    int secondary_thread_ttl_ = CGRAPH_SECONDARY_THREAD_TTL;
    int monitor_span_ = CGRAPH_MONITOR_SPAN;
    bool batch_task_enable_ = CGRAPH_BATCH_TASK_ENABLE;
    bool fair_lock_enable_ = CGRAPH_FAIR_LOCK_ENABLE;


    /**
     * 设置默认信息
     */
    void reset() {
        default_thread_size_ = CGRAPH_DEFAULT_THREAD_SIZE;
        max_thread_size_ = CGRAPH_MAX_THREAD_SIZE;
        max_task_steal_range_ = CGRAPH_MAX_TASK_STEAL_RANGE;
        max_local_batch_size_ = CGRAPH_MAX_LOCAL_BATCH_SIZE;
        max_pool_batch_size_ = CGRAPH_MAX_POOL_BATCH_SIZE;
        max_steal_batch_size_ = CGRAPH_MAX_STEAL_BATCH_SIZE;
        secondary_thread_ttl_ = CGRAPH_SECONDARY_THREAD_TTL;
        monitor_span_ = CGRAPH_MONITOR_SPAN;
        batch_task_enable_ = CGRAPH_BATCH_TASK_ENABLE;
        fair_lock_enable_ = CGRAPH_FAIR_LOCK_ENABLE;
    }


    /**
     * 计算可盗取的范围，盗取范围不能超过默认线程数-1
     * @return
     */
    [[nodiscard]] int calcStealRange() const {
        int range = std::min(this->max_task_steal_range_, this->default_thread_size_ - 1);
        return range;
    }


    /**
     * 计算是否开启批量任务
     * 开启条件：开关批量开启，并且 未开启非公平锁
     * @return
     */
    [[nodiscard]] bool calcBatchTaskRatio() const {
        bool ratio = (this->batch_task_enable_) && (!this->fair_lock_enable_);
        return ratio;
    }
};

using UThreadPoolConfigPtr = UThreadPoolConfig *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTHREADPOOLCONFIG_H
