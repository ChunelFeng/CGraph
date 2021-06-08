/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.cpp
@Time: 2021/6/2 10:15 下午
@Desc: 
***************************/

#include <set>
#include "GPipeline.h"

GPipeline::GPipeline() {
    thread_pool_ = new(std::nothrow) GraphThreadPool();
    manager_ = new(std::nothrow) GElementManager();
    is_init_ = false;
}


GPipeline::~GPipeline() {
    CGRAPH_DELETE_PTR(thread_pool_)
    CGRAPH_DELETE_PTR(manager_)

    // 结束的时候，清空所有创建的节点信息。所有节点信息，仅在这一处释放
    for (GElementPtr element : element_repository_) {
        CGRAPH_DELETE_PTR(element);
    }
}


CSTATUS GPipeline::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    status = manager_->init();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CSTATUS GPipeline::run() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    int runElementSize = 0;
    std::vector<std::future<CSTATUS>> futures;

    for (GClusterArr& clusterArr : manager_->para_cluster_arrs_) {
        futures.clear();

        for (GCluster& cluster : clusterArr) {
            futures.push_back(std::move(this->thread_pool_->commit(cluster)));
            runElementSize += cluster.getElementNum();
        }

        for (auto& fut : futures) {
            status = fut.get();
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    status = manager_->afterRunCheck(runElementSize);
    CGRAPH_FUNCTION_END
}


CSTATUS GPipeline::deinit() {
    CGRAPH_FUNCTION_BEGIN

    status = manager_->deinit();
    CGRAPH_FUNCTION_END
}


CSTATUS GPipeline::addDependElements(GElementPtr element,
                                     const std::set<GElementPtr>& dependElements) const {
    CGRAPH_FUNCTION_BEGIN

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

