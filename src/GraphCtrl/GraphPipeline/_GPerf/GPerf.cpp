/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPerf.cpp
@Time: 2023/7/29 22:38
@Desc: 
***************************/

#include "GPerf.h"
#include "GPerfDefine.h"
#include "../GPipeline.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GPerf::perf(GPipelinePtr pipeline) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline)

    status = inject(pipeline);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = pipeline->process();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = pipeline->dump();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = recover(pipeline);
    CGRAPH_FUNCTION_CHECK_STATUS

    CGRAPH_FUNCTION_END
}


CStatus GPerf::inject(GPipelinePtr pipeline) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline)

    const CFMSec now = CGRAPH_GET_CURRENT_ACCURATE_MS();
    for (auto* cur : pipeline->repository_.elements_) {
        /**
         * 给其中的每个element，都添加这个切面信息
         * 这里是不需要考虑 delete perf_info_的，因为在 element结束的时候，会自动释放
         */
        cur->perf_info_ = UAllocator::safeMallocCStruct<GPerfInfo>();
        cur->addGAspect<GPerfAspect<CFMSec, GPerfInfoPtr>>(now, cur->perf_info_);
    }
    CGRAPH_FUNCTION_END
}


CStatus GPerf::recover(GPipelinePtr pipeline) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline)
    for (auto* cur : pipeline->repository_.elements_) {
        status += cur->popLastAspect();
        CGRAPH_DELETE_PTR(cur->perf_info_)
    }
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
