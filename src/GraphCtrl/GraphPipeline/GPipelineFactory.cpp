/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPipelineFactory.cpp
@Time: 2021/6/23 10:25 下午
@Desc: 
***************************/

#include "GPipelineFactory.h"

CGRAPH_NAMESPACE_BEGIN

GPipelinePtrList GPipelineFactory::s_pipeline_list_;
std::mutex GPipelineFactory::s_lock_;

GPipelinePtr GPipelineFactory::create() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_LOCK_GUARD lock(s_lock_);

    if (s_pipeline_list_.empty()) {
        UThreadPoolSingleton::get();    // 默认直接开启线程池了
    }

    auto pipeline = CGRAPH_SAFE_MALLOC_COBJECT(GPipeline)
    s_pipeline_list_.emplace_back(pipeline);
    return pipeline;
}


void GPipelineFactory::destroy(GPipelinePtr pipeline) {
    if (nullptr == pipeline) {
        return;
    }

    CGRAPH_LOCK_GUARD lock(s_lock_);
    s_pipeline_list_.remove(pipeline);
    CGRAPH_DELETE_PTR(pipeline)

    if (s_pipeline_list_.empty()) {
        UThreadPoolSingleton::get(false)->deinit();
    }
}


void GPipelineFactory::clear() {
    CGRAPH_LOCK_GUARD lock(s_lock_);

    for (GPipelinePtr pipeline : GPipelineFactory::s_pipeline_list_) {
        CGRAPH_DELETE_PTR(pipeline)
    }

    UThreadPoolSingleton::get(false)->deinit();
    s_pipeline_list_.clear();
}


CStatus GPipelineFactory::run() {
    CGRAPH_NO_SUPPORT
}

CGRAPH_NAMESPACE_END
