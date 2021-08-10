/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.cpp
@Time: 2021/6/2 10:15 下午
@Desc: 
***************************/

#include <set>
#include <functional>

#include "GPipeline.h"

GPipeline::GPipeline() {
    thread_pool_ = CGRAPH_SAFE_MALLOC_COBJECT(UThreadPool);
    element_manager_ = new(std::nothrow) GElementManager();
    param_manager_ = new(std::nothrow) GParamManager();
    is_init_ = false;
}


GPipeline::~GPipeline() {
    CGRAPH_DELETE_PTR(thread_pool_)
    CGRAPH_DELETE_PTR(element_manager_)
    CGRAPH_DELETE_PTR(param_manager_)

    // 结束的时候，清空所有创建的节点信息。所有节点信息，仅在这一处释放
    for (GElementPtr element : element_repository_) {
        CGRAPH_DELETE_PTR(element);
    }
}


CSTATUS GPipeline::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)

    status = element_manager_->init();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = thread_pool_->init();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CSTATUS GPipeline::run() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)

    int runElementSize = 0;
    std::vector<std::future<CSTATUS>> futures;

    for (GClusterArr& clusterArr : element_manager_->para_cluster_arrs_) {
        futures.clear();

        for (GCluster& cluster : clusterArr) {
            futures.emplace_back(thread_pool_->commit(std::bind(&GCluster::process, cluster, false)));
            runElementSize += cluster.getElementNum();
        }

        for (auto& fut : futures) {
            status = fut.get();
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    param_manager_->reset();
    status = element_manager_->afterRunCheck(runElementSize);
    CGRAPH_FUNCTION_END
}


CSTATUS GPipeline::deinit() {
    CGRAPH_FUNCTION_BEGIN

    status = thread_pool_->deinit();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->deinit();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = param_manager_->deinit();
    CGRAPH_FUNCTION_END
}


CSTATUS GPipeline::process(int runTimes) {
    CGRAPH_FUNCTION_BEGIN
    status = init();
    CGRAPH_FUNCTION_CHECK_STATUS

    while (runTimes-- > 0) {
        status = run();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    status = deinit();
    CGRAPH_FUNCTION_END
}


CSTATUS GPipeline::addDependElements(GElementPtr element,
                                     const std::set<GElementPtr>& dependElements) const {
    CGRAPH_FUNCTION_BEGIN

    // TODO 这个功能可以下沉到element里去实现
    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(element)

    for (GElementPtr cur : dependElements) {
        // 如果传入的信息中，有nullptr，则所有的信息均不参与计算
        CGRAPH_ASSERT_NOT_NULL(cur);
    }

    for (GElementPtr cur : dependElements) {
        if (cur == element) {
            continue;        // 本节点无法依赖本节点
        }

        cur->run_before_.insert(element);
        element->dependence_.insert(cur);
    }

    element->left_depend_ = element->dependence_.size();

    CGRAPH_FUNCTION_END
}

