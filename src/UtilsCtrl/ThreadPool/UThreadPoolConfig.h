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

struct UThreadPoolConfig : public CStruct {
    /** 具体值含义，参考UThreadPoolDefine.h文件 */
    int default_thread_size_ = CGRAPH_DEFAULT_THREAD_SIZE;
    int secondary_thread_size_ = CGRAPH_SECONDARY_THREAD_SIZE;
    int max_thread_size_ = CGRAPH_MAX_THREAD_SIZE;
    int max_task_steal_range_ = CGRAPH_MAX_TASK_STEAL_RANGE;
    int max_local_batch_size_ = CGRAPH_MAX_LOCAL_BATCH_SIZE;
    int max_pool_batch_size_ = CGRAPH_MAX_POOL_BATCH_SIZE;
    int max_steal_batch_size_ = CGRAPH_MAX_STEAL_BATCH_SIZE;
    int secondary_thread_ttl_ = CGRAPH_SECONDARY_THREAD_TTL;
    int monitor_span_ = CGRAPH_MONITOR_SPAN;
    CMSec queue_emtpy_interval_ = CGRAPH_QUEUE_EMPTY_INTERVAL;
    int primary_thread_policy_ = CGRAPH_PRIMARY_THREAD_POLICY;
    int secondary_thread_policy_ = CGRAPH_SECONDARY_THREAD_POLICY;
    int primary_thread_priority_ = CGRAPH_PRIMARY_THREAD_PRIORITY;
    int secondary_thread_priority_ = CGRAPH_SECONDARY_THREAD_PRIORITY;
    bool bind_cpu_enable_ = CGRAPH_BIND_CPU_ENABLE;
    bool batch_task_enable_ = CGRAPH_BATCH_TASK_ENABLE;
    bool monitor_enable_ = CGRAPH_MONITOR_ENABLE;

    CStatus check() const {
        CGRAPH_FUNCTION_BEGIN
        if (default_thread_size_ < 0 || secondary_thread_size_ < 0) {
            CGRAPH_RETURN_ERROR_STATUS("thread size cannot less than 0")
        }

        if (default_thread_size_ + secondary_thread_size_ > max_thread_size_) {
            CGRAPH_RETURN_ERROR_STATUS("max thread size is less than default + secondary thread")
        }

        if (monitor_enable_ && monitor_span_ <= 0) {
            CGRAPH_RETURN_ERROR_STATUS("monitor span cannot less than 0")
        }
        CGRAPH_FUNCTION_END
    }

protected:
    /**
     * 计算可盗取的范围，盗取范围不能超过默认线程数-1
     * @return
     */
    int calcStealRange() const {
        int range = std::min(this->max_task_steal_range_, this->default_thread_size_ - 1);
        return range;
    }

    friend class UThreadPrimary;
    friend class UThreadSecondary;
};

using UThreadPoolConfigPtr = UThreadPoolConfig *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTHREADPOOLCONFIG_H
