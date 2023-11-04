/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPerfAspect.h
@Time: 2023/7/29 23:28
@Desc: 
***************************/

#ifndef CGRAPH_GPERFASPECT_H
#define CGRAPH_GPERFASPECT_H

#include "../../GraphAspect/GAspectInclude.h"
#include "GPerfDefine.h"

CGRAPH_NAMESPACE_BEGIN

template <typename ...Args>
class GPerfAspect : public GTemplateAspect<Args ...> {
protected:
    explicit GPerfAspect(CFMSec startTs, GPerfInfoPtr perfInfo) {
        CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(perfInfo)
        pipeline_start_ts_ = startTs;
        perf_info_ = perfInfo;
    }

    CStatus beginRun() final {
        CGRAPH_FUNCTION_BEGIN

        cur_start_ts_ = CGRAPH_GET_CURRENT_ACCURATE_MS();
        if (0 == perf_info_->first_start_ts_) {
            // 记录开始的时间信息，仅记录第一次运行到这个node的时间信息
            perf_info_->first_start_ts_ = (cur_start_ts_ - pipeline_start_ts_);
        }
        CGRAPH_FUNCTION_END
    }

    CVoid finishRun(const CStatus& curStatus) final {
        auto cur = CGRAPH_GET_CURRENT_ACCURATE_MS();
        perf_info_->last_finish_ts_ = cur - pipeline_start_ts_;
        perf_info_->loop_++;
        perf_info_->accu_cost_ts_ += (cur - cur_start_ts_);
    }

private:
    CFMSec pipeline_start_ts_ = 0.0;                  // 流水线开启的时间
    CFMSec cur_start_ts_ = 0.0;                       // 当前element开始运行的时间
    GPerfInfoPtr perf_info_ = nullptr;                // 具体赋值的对象

    friend class UAllocator;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPERFASPECT_H
