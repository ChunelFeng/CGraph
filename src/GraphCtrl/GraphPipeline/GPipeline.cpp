/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.cpp
@Time: 2021/6/2 10:15 下午
@Desc: 
***************************/

#include "GPipeline.h"

CGRAPH_NAMESPACE_BEGIN

GPipeline::GPipeline() {
    thread_pool_ = UThreadPoolSingleton::get();
    element_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GElementManager)
    param_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GParamManager)
    daemon_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GDaemonManager)
    is_init_ = false;
}


GPipeline::~GPipeline() {
    CGRAPH_DELETE_PTR(daemon_manager_)
    CGRAPH_DELETE_PTR(element_manager_)
    CGRAPH_DELETE_PTR(param_manager_)

    // 结束的时候，清空所有创建的节点信息。所有节点信息，仅在这一处释放
    for (GElementPtr element : element_repository_) {
        CGRAPH_DELETE_PTR(element)
    }
}


CStatus GPipeline::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)
    CGRAPH_ASSERT_NOT_NULL(daemon_manager_)

    status = element_manager_->init();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = daemon_manager_->init();    // daemon的初始化，需要晚于所有element的初始化
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::run() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)

    CSize runElementSize = 0;    // 用于记录执行的element的总数，用于后期校验
    std::vector<CMSec> curClusterTtl;    // 用于记录分解后，每个cluster包含的element的个数，用于验证执行的超时情况。
    std::vector<std::future<CStatus>> futures;

    for (GClusterArrRef clusterArr : element_manager_->para_cluster_arrs_) {
        futures.clear();
        curClusterTtl.clear();

        /** 将分解后的pipeline信息，以cluster为维度，放入线程池依次执行 */
        for (GClusterRef cluster : clusterArr) {
            futures.emplace_back(thread_pool_->commit(std::bind(&GCluster::process, std::ref(cluster), false)));
            runElementSize += cluster.getElementNum();
            curClusterTtl.emplace_back(cluster.getElementNum() * element_run_ttl_);
        }

        int index = 0;
        for (auto& fut : futures) {
            // 不设定最大运行周期的情况（默认情况）
            if (likely(CGRAPH_DEFAULT_ELEMENT_RUN_TTL == element_run_ttl_)) {
                status += fut.get();
            } else {
                // 如果设定超时时间，则以超时时间为准
                const auto& futStatus = fut.wait_for(std::chrono::milliseconds(curClusterTtl[index]));
                switch (futStatus) {
                    case std::future_status::ready: status += fut.get(); break;
                    case std::future_status::timeout: status += CStatus("thread status timeout"); break;
                    case std::future_status::deferred: status += CStatus("thread status deferred"); break;
                    default: status += CStatus("thread status unknown");
                }
            }
            index++;
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    param_manager_->reset();
    status = element_manager_->afterRunCheck(runElementSize);
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::destroy() {
    CGRAPH_FUNCTION_BEGIN

    status = daemon_manager_->destroy();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->destroy();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = param_manager_->destroy();
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::process(CSize runTimes) {
    CGRAPH_FUNCTION_BEGIN
    status = init();
    CGRAPH_FUNCTION_CHECK_STATUS

    while (runTimes-- > 0) {
        status = run();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    status = destroy();
    CGRAPH_FUNCTION_END
}


GPipelinePtr GPipeline::setGElementRunTtl(CMSec ttl) {
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)

    this->element_run_ttl_ = ttl;
    return this;
}

CGRAPH_NAMESPACE_END
