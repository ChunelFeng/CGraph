/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GRegion.cpp
@Time: 2021/6/1 10:14 下午
@Desc: 
***************************/

#include <functional>

#include "GRegion.h"

CGRAPH_NAMESPACE_BEGIN

GRegion::GRegion() : GGroup() {
    manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GElementManager)
    thread_pool_ = nullptr;
    is_init_ = false;
}


GRegion::~GRegion() {
    CGRAPH_DELETE_PTR(manager_)
}


GRegion::GRegion(const GRegion& region) : GGroup(region) {
    for (GElementPtr element : region.manager_->manager_elements_) {
        this->manager_->manager_elements_.insert(element);
    }

    this->manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GElementManager);
    for (auto element : region.manager_->manager_elements_) {
        this->manager_->manager_elements_.insert(element);
    }

    this->thread_pool_ = region.thread_pool_;
}


GRegion& GRegion::operator=(const GRegion& region){
    if (this == &region) {
        return (*this);
    }

    this->manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GElementManager)
    for (auto element : region.manager_->manager_elements_) {
        this->manager_->manager_elements_.insert(element);
    }
    this->thread_pool_ = region.thread_pool_;

    return (*this);
}


CStatus GRegion::init() {
    CGRAPH_FUNCTION_BEGIN
    // 在这里将初始化所有的节点信息，并且实现分析，联通等功能
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    status = this->manager_->init();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CStatus GRegion::deinit() {
    CGRAPH_FUNCTION_BEGIN
    status = manager_->deinit();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CStatus GRegion::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    int runNodeSize = 0;
    std::vector<std::future<CStatus>> futures;

    for (GClusterArr& clusterArr : manager_->para_cluster_arrs_) {
        futures.clear();

        for (GCluster& cluster : clusterArr) {
            futures.emplace_back(thread_pool_->commit(std::bind(&GCluster::process, std::ref(cluster), false)));
            runNodeSize += cluster.getElementNum();
        }

        for (auto& fut : futures) {
            status = fut.get();
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    status = manager_->afterRunCheck(runNodeSize);
    CGRAPH_FUNCTION_END
}


CStatus GRegion::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_NOT_NULL(element)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    manager_->manager_elements_.emplace(element);
    CGRAPH_FUNCTION_END
}


CStatus GRegion::setThreadPool(UThreadPoolPtr pool) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pool)

    this->thread_pool_ = pool;
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END