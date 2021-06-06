/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.cpp
@Time: 2021/6/2 10:15 下午
@Desc: 
***************************/

#include <set>
#include "GFlow.h"

GFlow::GFlow() {
    thread_pool_ = new(std::nothrow) GraphThreadPool();
    manager_ = new(std::nothrow) GElementManager();
    is_init_ = false;
}


GFlow::~GFlow() {
    CGRAPH_DELETE_PTR(thread_pool_)
    CGRAPH_DELETE_PTR(manager_)

    // 结束的时候，清空所有创建的节点信息。所有节点信息，仅在这一处释放
    for (GElementPtr element : element_repository_) {
        CGRAPH_DELETE_PTR(element);
    }
}


CSTATUS GFlow::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    status = manager_->init();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = analyse();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CSTATUS GFlow::run() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    int runElementSize = 0;
    std::vector<std::future<CSTATUS>> futures;

    for (GClusterArr& clusterArr : para_cluster_arrs_) {
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

    status = checkFinalStatus(runElementSize);

    CGRAPH_FUNCTION_END
}


CSTATUS GFlow::deinit() {
    CGRAPH_FUNCTION_BEGIN

    status = manager_->deinit();
    CGRAPH_FUNCTION_END
}


CSTATUS GFlow::analyse() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(false)
    int runElementSize = 0;
    int totalElementSize = manager_->manager_elements_.size();

    GClusterArr curClusterArr;    // 记录每一层，可以并行的逻辑
    for (GElementPtr element : manager_->manager_elements_) {
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

                        while (curElement->isLinkable()
                               && 1 == curElement->run_before_.size()
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


CSTATUS GFlow::addDependElements(GElementPtr element,
                                 const std::set<GElementPtr>& dependElements) const {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(element)

    for (GElementPtr cur : dependElements) {
        // 如果传入的信息中，有nullptr，则所有的信息均不参与计算
        CGRAPH_ASSERT_NOT_NULL(cur);
    }

    for (const GElementPtr cur : dependElements) {
        if (cur == element) {
            continue;        // 本节点无法依赖本节点
        }

        cur->run_before_.insert(element);
        element->dependence_.insert(cur);
    }

    element->left_depend_ = element->dependence_.size();

    CGRAPH_FUNCTION_END
}


CSTATUS GFlow::checkFinalStatus(int runNodeSize) {
    CGRAPH_FUNCTION_BEGIN

    status = (runNodeSize == manager_->manager_elements_.size()) ? STATUS_OK : STATUS_ERR;
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


GClusterPtr GFlow::createGCluster(const GElementPtrArr& elements,
                                  const GElementPtrSet& dependElements,
                                  const std::string& name,
                                  int loop) {
    // 如果有一个element为null，则创建失败
    if (std::any_of(elements.begin(), elements.end(),
                    [](GElementPtr element) {
        return (nullptr == element);
    })) {
        return nullptr;
    }

    if (std::any_of(dependElements.begin(), dependElements.end(),
                    [](GElementPtr element) {
            return (nullptr == element);
        })) {
        return nullptr;
    }

    auto cluster = new(std::nothrow) GCluster();
    if (nullptr == cluster) {
        return nullptr;
    }

    for (const GElementPtr element : elements) {
        cluster->addElement(element);
    }

    cluster->setName(name);
    cluster->setLoop(loop);
    cluster->dependence_ = dependElements;

    this->element_repository_.insert(cluster);
    return cluster;
}


GRegionPtr GFlow::createGRegion(const GElementPtrArr& elements,
                                const GElementPtrSet& dependElements,
                                const std::string& name,
                                int loop) {
    // 如果有一个element为null，则创建失败
    if (std::any_of(elements.begin(), elements.end(),
                    [](GElementPtr element) {
        return (nullptr == element);
    })) {
        return nullptr;
    }

    auto region = new(std::nothrow) GRegion();
    if (nullptr == region) {
        return nullptr;
    }

    for (const GElementPtr element : elements) {
        // region与cluster的区别，cluster是线性的，但是region是通过manager类来管理element的
        region->manager_->addElement(element);
    }

    region->dependence_ = dependElements;
    region->name_ = name;
    region->loop_ = loop;
    region->setThreadPool(this->thread_pool_);
    this->element_repository_.insert(region);
    return region;
}