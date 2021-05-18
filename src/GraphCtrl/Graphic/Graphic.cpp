/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.cpp
@Time: 2021/4/26 9:16 下午
@Desc: 构建图环境信息
***************************/

#include <set>
#include <memory>
#include <vector>
#include "Graphic.h"

Graphic::Graphic() {
    thread_pool_ = std::make_unique<GraphThreadPool>();
    node_manage_ = std::make_unique<GraphNodeManager>();
    is_init_ = false;
}

Graphic::~Graphic() {
    is_init_ = false;
}

CSTATUS Graphic::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(node_manage_)

    status = node_manage_->init();
    CGRAPH_FUNCTION_CHECK_STATUS
    is_init_ = true;    // 初始化完毕，设置标记位
    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::deinit() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)

    status = node_manage_->deinit();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::run() {
    CGRAPH_FUNCTION_BEGIN

    // 必须初始化之后，才可以执行run方法
    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(node_manage_)

    int runNodeSize = 0;
    int totalNodeSize = node_manage_->graph_nodes_.size();
    for (GraphNode* node : node_manage_->graph_nodes_) {
        if (!node->isRunnable() || node->isLinkable()) {
            continue;    // 如果暂时无法执行，或者属于可以连接的，则不记录
        }

        GraphNodeCluster curCluster;    // 这里是临时变量，小心一点为好
        GraphNode* curNode = node;
        curCluster.addNode(curNode);

        /* 将linkable的节点，统一放到一个cluster中 */
        while (1 == curNode->run_before_.size()
               && (*curNode->run_before_.begin())->isLinkable()) {
            curNode = (*curNode->run_before_.begin());
            curCluster.addNode(curNode);
        }

        cluster_queue_.emplace(curCluster);
    }

    std::vector<GraphNodeCluster> runnableClusters;    // mark，这里传递值不是很合理，今后有空修改一下
    std::vector<std::future<CSTATUS>> futures;

    while (!cluster_queue_.empty() || runNodeSize > totalNodeSize) {
        runnableClusters.clear();
        futures.clear();

        while(!cluster_queue_.empty()) {
            runNodeSize += cluster_queue_.front().size();    // 记录运行的数字
            runnableClusters.emplace_back(cluster_queue_.front());
            cluster_queue_.pop();
        }

        for (GraphNodeCluster& cluster : runnableClusters) {
            // 放到线程池中执行
            futures.emplace_back(std::move(thread_pool_->commit(cluster)));
        }

        /* 这里相当于做了一层屏障，所有运行的线程停止后，再往下执行 */
        for (auto& fut : futures) {
            status = fut.get();
            CGRAPH_FUNCTION_CHECK_STATUS
        }

        std::set<GraphNode *> duplications;    // 防止当前流程中，单个节点重复添加，做去重使用
        for (GraphNodeCluster& cluster : runnableClusters) {
            /* 把执行过的节点取出来，查看他们的后继节点是否有可以运行的 */
            for (GraphNode* node : cluster.getNodes()) {
                for (GraphNode* cur : node->run_before_) {
                    /**
                     * 判断node是否需要被加入queue_中进行下一轮循环的条件
                     * 1，该node是可以执行的
                     * 2，该node是非联通节点（联通节点会跟随其依赖节点，直接放入cluster中）
                     * 3，该node本次循环是第一次被遍历
                     */
                    if (cur->isRunnable()
                        && !cur->isLinkable()
                        && duplications.find(cur) == duplications.end()) {

                        GraphNodeCluster curCluster;
                        GraphNode* curNode = cur;
                        curCluster.addNode(curNode);
                        duplications.insert(curNode);

                        while (1 == curNode->run_before_.size()
                               && (*curNode->run_before_.begin())->isLinkable()) {
                            curNode = (*curNode->run_before_.begin());
                            curCluster.addNode(curNode);
                            duplications.insert(curNode);
                        }
                        cluster_queue_.emplace(curCluster);
                    }
                }
            }
        }
    }

    // 执行完毕后，确认是否所有节点均被执行过一次
    status = checkFinalStatus(runNodeSize);

    CGRAPH_FUNCTION_END
}

/**
 * 设置
 * @param node
 * @param dependNodes
 * @return
 */
CSTATUS Graphic::addDependNodes(GraphNode* node,
                                const std::set<GraphNode *>& dependNodes) const {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(node)

    for (GraphNode* cur : dependNodes) {
        // 如果传入的信息中，有nullptr，则所有的信息均不参与计算
        CGRAPH_ASSERT_NOT_NULL(cur);
    }

    for (GraphNode* cur : dependNodes) {
        if (cur == node) {
            continue;        // 本节点无法依赖本节点
        }

        cur->run_before_.insert(node);
        node->dependence_.insert(cur);
    }

    node->left_depend_ = node->dependence_.size();

    CGRAPH_FUNCTION_END
}


/** protected **/
CSTATUS Graphic::checkFinalStatus(int runNodeSize) {
    CGRAPH_FUNCTION_BEGIN

    // 判定是否是所有的节点均被执行1次
    status = (runNodeSize == node_manage_->graph_nodes_.size()) ? STATUS_OK : STATUS_ERR;
    CGRAPH_FUNCTION_CHECK_STATUS

    // 判定是否所有节点均执行到了
    for (GraphNode* node : node_manage_->graph_nodes_) {
        if (false == node->done_) {
            status = STATUS_ERR;
        }

        // 将节点信息复位，以便于开始下一次循环
        node->done_.store(false);
        node->left_depend_ = node->dependence_.size();
    }

    CGRAPH_FUNCTION_END
}

