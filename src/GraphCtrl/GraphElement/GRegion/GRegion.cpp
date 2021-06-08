/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GRegion.cpp
@Time: 2021/6/1 10:14 下午
@Desc: 
***************************/

#include "GRegion.h"

GRegion::GRegion() : GElement() {
    manager_ = new(std::nothrow) GElementManager();
    thread_pool_ = nullptr;
    is_init_ = false;
}


GRegion::~GRegion() {
    CGRAPH_DELETE_PTR(manager_)
}


GRegion::GRegion(const GRegion& region) {
    for (GElementPtr element : region.manager_->manager_elements_) {
        this->manager_->manager_elements_.insert(element);
    }

    this->manager_ = region.manager_;
    this->thread_pool_ = region.thread_pool_;
}


GRegion& GRegion::operator=(const GRegion& region){
    if (this == &region) {
        return (*this);
    }

    for (GElementPtr element : region.manager_->manager_elements_) {
        this->manager_->manager_elements_.insert(element);
    }

    this->manager_ = region.manager_;
    this->thread_pool_ = region.thread_pool_;

    return (*this);
}


CSTATUS GRegion::init() {
    CGRAPH_FUNCTION_BEGIN
    // 在这里将初始化所有的节点信息，并且实现分析，联通等功能
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    status = this->manager_->init();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CSTATUS GRegion::deinit() {
    CGRAPH_FUNCTION_BEGIN
    status = manager_->deinit();

    CGRAPH_FUNCTION_END
}


CSTATUS GRegion::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    int runNodeSize = 0;
    std::vector<std::future<CSTATUS>> futures;

    for (GClusterArr& clusterArr : manager_->para_cluster_arrs_) {
        futures.clear();

        for (GCluster& cluster : clusterArr) {
            futures.push_back(std::move(this->thread_pool_->commit(cluster)));
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


CSTATUS GRegion::process(bool isMock) {
    CGRAPH_FUNCTION_BEGIN
    status = this->beforeRun();
    CGRAPH_FUNCTION_CHECK_STATUS
    if (!isMock) {
        // 运行region中的信息。这里的信息，已经提前被解析了。
        status = run();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    status = this->afterRun();
    CGRAPH_FUNCTION_END
}


CSTATUS GRegion::beforeRun() {
    CGRAPH_FUNCTION_BEGIN

    this->done_ = false;
    this->left_depend_ = this->dependence_.size();
    for (GElementPtr element : this->manager_->manager_elements_) {
        status = element->beforeRun();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CSTATUS GRegion::afterRun() {
    CGRAPH_FUNCTION_BEGIN
    for (GElementPtr element : this->region_elements_) {
        status = element->afterRun();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    for (auto& element : this->run_before_) {
        element->left_depend_--;
    }
    this->done_ = true;
    CGRAPH_FUNCTION_END
}


CSTATUS GRegion::setThreadPool(GraphThreadPool* pool) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pool)

    this->thread_pool_ = pool;
    CGRAPH_FUNCTION_END
}
