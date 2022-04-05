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
    is_init_ = false;
}


GRegion::~GRegion() {
    CGRAPH_DELETE_PTR(manager_)
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


CStatus GRegion::destroy() {
    CGRAPH_FUNCTION_BEGIN
    status = manager_->destroy();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CStatus GRegion::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    do {
        /**
         * region内部方法，至少执行一次
         * 如果有等待条件终止的逻辑，需要继承region，并且复写 isHold()方法
         */
        CSize runNodeSize = 0;
        std::vector<std::future<CStatus> > futures;

        for (GClusterArrRef clusterArr : manager_->para_cluster_arrs_) {
            futures.clear();

            for (GClusterRef cluster : clusterArr) {
                futures.emplace_back(thread_pool_->commit(std::bind(&GCluster::process, std::ref(cluster), false)));
                runNodeSize += cluster.getElementNum();
            }

            for (auto& fut : futures) {
                status = fut.get();
                CGRAPH_FUNCTION_CHECK_STATUS
            }
        }

        status = manager_->afterRunCheck(runNodeSize);
        CGRAPH_FUNCTION_CHECK_STATUS
    } while (isHold());    // 判定是否需要结束执行，默认执行一次结束

    CGRAPH_FUNCTION_END
}


CStatus GRegion::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_NOT_NULL(element)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    manager_->manager_elements_.emplace(element);
    CGRAPH_FUNCTION_END
}


CBool GRegion::isHold() {
    /**
     * 传统的region，默认不需要这种重复执行的机制。
     * 开放出来用于扩展
     */
    return false;
}

CGRAPH_NAMESPACE_END