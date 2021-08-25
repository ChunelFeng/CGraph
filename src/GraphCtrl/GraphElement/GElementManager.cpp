/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementManager.cpp
@Time: 2021/6/2 10:33 下午
@Desc: 
***************************/

#include "GElementManager.h"

GElementManager::GElementManager() : CObject() {

}

GElementManager::~GElementManager() = default;


GElementManager::GElementManager(const GElementManager& manager) {
    this->manager_elements_ = manager.manager_elements_;
}


GElementManager& GElementManager::operator=(const GElementManager& manager) {
    if (this == &manager) {
        return (*this);
    }

    this->manager_elements_ = manager.manager_elements_;
    return (*this);
}


CSTATUS GElementManager::init() {
    CGRAPH_FUNCTION_BEGIN
    status = preRunCheck();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = analyse();
    CGRAPH_FUNCTION_CHECK_STATUS

    // manager 中的所有element初始化
    status = std::all_of(manager_elements_.begin(), manager_elements_.end(),
                         [](GElementPtr element) {
        return STATUS_OK == element->init();
    }) ? STATUS_OK : STATUS_ERR;

    CGRAPH_FUNCTION_END
}


CSTATUS GElementManager::deinit() {
    CGRAPH_FUNCTION_BEGIN

    status = std::all_of(manager_elements_.begin(), manager_elements_.end(),
                         [](GElementPtr element) {
        return STATUS_OK == element->deinit();
    }) ? STATUS_OK : STATUS_ERR;

    CGRAPH_FUNCTION_END
}


/**
 * 无法执行manager的run方法
 * @return
 */
CSTATUS GElementManager::run() {
    CGRAPH_NO_SUPPORT
}


CSTATUS GElementManager::preRunCheck() {
    CGRAPH_FUNCTION_BEGIN

    /**
     * 认定图可以连通的判定条件：
     * 1，当前节点仅有一个依赖
     * 2，当前节点依赖的节点，只有一个后继
     * 3，当前节点的依赖的后继，仍是当前节点
     */
    for (GElement* element : manager_elements_) {
        if (1 == element->dependence_.size()
            && 1 == (*element->dependence_.begin())->run_before_.size()
            && (*(element->dependence_.begin()))->run_before_.find(element) != (*(element->dependence_.begin()))->run_before_.end()) {
            element->linkable_ = true;
        }
    }

    CGRAPH_FUNCTION_END
}


CSTATUS GElementManager::analyse() {
    CGRAPH_FUNCTION_BEGIN

    int runElementSize = 0;
    int totalElementSize = manager_elements_.size();
    para_cluster_arrs_.clear();

    GClusterArr curClusterArr;    // 记录每一层，可以并行的逻辑
    for (GElementPtr element : manager_elements_) {
        if (!element->isRunnable() || element->isLinkable()) {
            continue;
        }

        GCluster curCluster;
        GElementPtr curElement = element;
        curCluster.addElement(curElement);

        /* 将linkable的节点，统一放到一个cluster中 */
        while (1 == curElement->run_before_.size()
               && (*curElement->run_before_.begin())->isLinkable()) {
            // 将下一个放到cluster中处理
            curElement = (*curElement->run_before_.begin());
            curCluster.addElement(curElement);
        }
        curClusterArr.emplace_back(curCluster);
    }
    para_cluster_arrs_.emplace_back(curClusterArr);

    GClusterArr runnableClusterArr;
    while (!curClusterArr.empty() && runElementSize <= totalElementSize) {
        runnableClusterArr = curClusterArr;
        curClusterArr.clear();

        for (GCluster& cluster : runnableClusterArr) {
            status = cluster.process(true);    // 不执行run方法的process
            CGRAPH_FUNCTION_CHECK_STATUS
        }
        runElementSize += runnableClusterArr.size();

        std::set<GElementPtr> duplications;
        for (GCluster& cluster : runnableClusterArr) {
            for (GElementPtr element : cluster.cluster_elements_) {
                for (GElementPtr cur : element->run_before_) {
                    /**
                     * 判断element是否需要被加入
                     * 1，该元素是可以执行的
                     * 2，改元素本次循环是第一次被遍历
                     */
                    if (cur->isRunnable()
                        && duplications.find(cur) == duplications.end()) {
                        GCluster curCluster;
                        GElementPtr curElement = cur;
                        curCluster.addElement(curElement);
                        duplications.insert(curElement);

                        while (1 == curElement->run_before_.size()
                               && (*curElement->run_before_.begin())->isLinkable()) {
                            curElement = (*curElement->run_before_.begin());
                            curCluster.addElement(curElement);
                            duplications.insert(curElement);
                        }
                        curClusterArr.emplace_back(curCluster);
                    }
                }
            }
        }

        /* 为空的话，直接退出循环；不为空的话，放入para信息中 */
        if (!curClusterArr.empty()) {
            para_cluster_arrs_.emplace_back(curClusterArr);
        }
    }

    CGRAPH_FUNCTION_END
}


CSTATUS GElementManager::afterRunCheck(int runNodeSize) {
    CGRAPH_FUNCTION_BEGIN

    status = (runNodeSize == manager_elements_.size()) ? STATUS_OK : STATUS_ERR;
    CGRAPH_FUNCTION_CHECK_STATUS

    /* 需要验证每个cluster里的每个内容是否被执行过一次 */
    for (GClusterArr& clusterArr : para_cluster_arrs_) {
        for (GCluster& cluster : clusterArr) {
            status = cluster.isElementsDone() ? STATUS_OK : STATUS_ERR;
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    CGRAPH_FUNCTION_END
}


CSTATUS GElementManager::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    this->manager_elements_.emplace(element);

    CGRAPH_FUNCTION_END
}


bool GElementManager::hasElement(GElementPtr element) const {
    if (nullptr == element) {
        return false;
    }

    return manager_elements_.find(element) != manager_elements_.end();
}


void GElementManager::deleteElement(GElementPtr element) {
    manager_elements_.erase(element);
}
