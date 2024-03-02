/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UMetrics.h
@Time: 2024/2/15 15:30
@Desc: 
***************************/

#ifndef CGRAPH_UMETRICS_H
#define CGRAPH_UMETRICS_H

#include <ostream>
#include <string>

#include "../UThreadObject.h"

CGRAPH_NAMESPACE_BEGIN

class UMetrics : public CStruct {
protected:
    explicit UMetrics() = default;

    /**
     * 计算thread 本地抓取的信息
     * @param result
     * @param size
     * @return
     */
    inline CVoid calcLocal(CBool result, CSize size) {
#ifndef _CGRAPH_SHOW_THREAD_METRICS_
        return;
#endif
        if (result) {
            local_pop_real_num_ += size;
        }
        local_pop_times_++;
    }

    /**
     * 计算thread 在全局队列中抓取的信息
     * @param result
     * @param size
     * @return
     */
    inline CVoid calcPool(CBool result, CSize size) {
#ifndef _CGRAPH_SHOW_THREAD_METRICS_
        return;
#endif
        if (result) {
            pool_pop_real_num_ += size;
        }
        pool_pop_times_++;
    }

    /**
     * 计算thread 在偷盗的信息
     * @param result
     * @param size
     * @return
     */
    inline CVoid calcSteal(CBool result, CSize size) {
#ifndef _CGRAPH_SHOW_THREAD_METRICS_
        return;
#endif
        if (result) {
            steal_pop_real_num_ += size;
        }
        steal_pop_times_++;
    }

    /**
     * 展示对应的结果信息
     * @param key
     * @return
     * @notice 不确保所有数据完全正确，主要是本人做性能调优分析和使用
     */
    CVoid show(const std::string& tag) const {
#ifndef _CGRAPH_SHOW_THREAD_METRICS_
        return;
#endif
    std::cout << tag
              << ": local_pop_real_num_ " << local_pop_real_num_
              << ", local_pop_times_ " << local_pop_times_
              << ", pool_pop_real_num_ " << pool_pop_real_num_
              << ", pool_pop_times_ " << pool_pop_times_
              << ", steal_pop_real_num_ " << steal_pop_real_num_
              << ", steal_pop_times_ " << steal_pop_times_
              << "\n \t fleet_wait_times_ " << fleet_wait_times_
              << ", deep_wait_times_ " << deep_wait_times_
              << ", local_push_real_num_ " << local_push_real_num_
              << ", local_push_yield_times_ " << local_push_yield_times_
              << "\n \t ";

    float localSuccessRate = local_pop_times_ > 0 ? float(local_pop_real_num_) / float(local_pop_times_) : 0.0f;
    float poolSuccessRate = pool_pop_times_ > 0 ? float(pool_pop_real_num_) / float(pool_pop_times_) : 0.0f;
    float stealSuccessRate = steal_pop_times_ > 0 ? float(steal_pop_real_num_) / float(steal_pop_times_) : 0.0f;
    float pushYieldRate = local_push_yield_times_ > 0 ? float(local_push_yield_times_) / float(local_push_real_num_) : 0.0f;

    printf("local success rate is [%0.3f], pool success rate is [%0.3f], "    \
           "steal success rate is [%0.3f], push yield rate is [%0.4f] \n\n",    \
           localSuccessRate, poolSuccessRate, stealSuccessRate, pushYieldRate);
    }

    /**
     * 恢复所有的数据信息
     * @return
     */
    CVoid reset() {
        local_pop_real_num_ = 0;
        local_pop_times_ = 0;
        pool_pop_real_num_ = 0;
        pool_pop_times_ = 0;
        steal_pop_real_num_ = 0;
        steal_pop_times_ = 0;
        local_push_real_num_ = 0;
        local_push_yield_times_ = 0;
        fleet_wait_times_ = 0;
        deep_wait_times_ = 0;
    }

private:
    CSize local_pop_real_num_ = 0;        // 本地pop出来数据个数
    CSize local_pop_times_ = 0;           // 本地尝试pop的次数
    CSize pool_pop_real_num_ = 0;         // 从pool中pop出来的数据个数
    CSize pool_pop_times_ = 0;            // 从pool中尝试pop的次数
    CSize steal_pop_real_num_ = 0;        // 偷盗获取的数据的个数
    CSize steal_pop_times_ = 0;           // 偷盗的次数
    CSize local_push_real_num_ = 0;       // 写入的真实次数
    CSize local_push_yield_times_ = 0;    // 写入冲突导致yield的次数
    CSize fleet_wait_times_ = 0;          // 轻量级等待的次数
    CSize deep_wait_times_ = 0;           // 深度等待的次数（触发了cv的wait机制）

    friend class UThreadBase;
    friend class UThreadPrimary;
    friend class UThreadSecondary;
    friend class UThreadPool;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UMETRICS_H
