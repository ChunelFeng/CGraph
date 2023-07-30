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

CStatus GPerf::perf(GPipeline* pipeline) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline)
    if (pipeline->is_init_) {
        CGRAPH_RETURN_ERROR_STATUS("pipeline is init, cannot perf it")
    }
    /**
     * 1. 将pipeline中，所有元素注入计时的切面信息
     * 2. 初始化pipeline
     * 3. 执行pipeline
     * 4. 打印出来耗时信息
     * 5. 销毁pipeline
     */

    status = inject(pipeline);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = pipeline->init();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = pipeline->run();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = pipeline->dump();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = pipeline->destroy();
    CGRAPH_FUNCTION_END
}


CStatus GPerf::inject(GPipeline* pipeline) {
    CGRAPH_FUNCTION_BEGIN
    const CMSec pipelineStartTs = CGRAPH_GET_CURRENT_MS();
    for (auto* cur : pipeline->repository_.elements_) {
        /**
         * 给其中的每个element，都添加这个切面信息
         * 这里是不需要考虑 delete perf_info_的，因为在 element结束的时候，会自动释放
         */
        cur->perf_info_ = UAllocator::safeMallocCStruct<GPerfInfo>();
        cur->addGAspect<GPerfAspect<CMSec, GPerfInfoPtr>>(pipelineStartTs, cur->perf_info_);
    }

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
