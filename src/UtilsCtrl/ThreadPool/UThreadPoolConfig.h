/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPoolConfig.h
@Time: 2022/1/3 9:31 下午
@Desc: 线程池配置信息
***************************/

#ifndef CGRAPH_UTHREADPOOLCONFIG_H
#define CGRAPH_UTHREADPOOLCONFIG_H

#include "UThreadObject.h"
#include "UThreadPoolDefine.h"

CGRAPH_NAMESPACE_BEGIN

struct UThreadPoolConfig : public UThreadObject {
    /** 具体值含义，参考UThreadPoolDefine.h文件 */
    int default_thread_size_ = CGRAPH_DEFAULT_THREAD_SIZE;
    int max_thread_size_ = CGRAPH_MAX_THREAD_SIZE;
    int max_task_steal_range_ = CGRAPH_MAX_TASK_STEAL_RANGE;
    int max_local_batch_size_ = CGRAPH_MAX_LOCAL_BATCH_SIZE;
    int max_pool_batch_size_ = CGRAPH_MAX_POOL_BATCH_SIZE;
    int max_steal_batch_size_ = CGRAPH_MAX_STEAL_BATCH_SIZE;
    int secondary_thread_ttl_ = CGRAPH_SECONDARY_THREAD_TTL;
    int monitor_span_ = CGRAPH_MONITOR_SPAN;
    int primary_thread_policy_ = CGRAPH_PRIMARY_THREAD_POLICY;
    int secondary_thread_policy_ = CGRAPH_SECONDARY_THREAD_POLICY;
    int primary_thread_priority_ = CGRAPH_PRIMARY_THREAD_PRIORITY;
    int secondary_thread_priority_ = CGRAPH_SECONDARY_THREAD_PRIORITY;
    bool bind_cpu_enable_ = CGRAPH_BIND_CPU_ENABLE;
    bool batch_task_enable_ = CGRAPH_BATCH_TASK_ENABLE;
    bool fair_lock_enable_ = CGRAPH_FAIR_LOCK_ENABLE;
    bool monitor_enable_ = CGRAPH_MONITOR_ENABLE;


protected:
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

    friend class UThreadPrimary;
    friend class UThreadSecondary;
};

using UThreadPoolConfigPtr = UThreadPoolConfig *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTHREADPOOLCONFIG_H
