/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.cpp
@Time: 2021/4/26 9:16 下午
@Desc: 构建图环境信息
***************************/

#include <set>
#include <memory>
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

    // 需要在init的时候，将graphic解析成多个cluster，放到vector中，标记好循环的次数
    status = analyse();
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
    std::vector<std::future<CSTATUS>> futures;
    for (GraphNodeLoopRegion& loopRegion : loop_regions_) {
        int loop = loopRegion.loop_;
        while (loop--) {
            for (ClusterArr& clusterArr : loopRegion.para_worked_clusters_) {
                futures.clear();

                for (GraphNodeCluster& cluster : clusterArr) {
                    futures.emplace_back(std::move(thread_pool_->commit(cluster)));
                    if (loop == 0) {
                        /* 执行到最后一次的时候，记录执行的次数 */
                        runNodeSize += cluster.size();
                    }
                }

                for (auto& fut : futures) {
                    status = fut.get();
                    CGRAPH_FUNCTION_CHECK_STATUS
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
CSTATUS Graphic::addDependNodes(GraphNodePtr node,
                                const std::set<GraphNodePtr>& dependNodes) const {
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

CSTATUS Graphic::setLoopRegion(const GraphNodePtr start,
                               const GraphNodePtr end,
                               int loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(start)
    CGRAPH_ASSERT_NOT_NULL(end)

    if (node_manage_->graph_nodes_.find(start) == node_manage_->graph_nodes_.end()
        || node_manage_->graph_nodes_.find(end) == node_manage_->graph_nodes_.end()) {
        return STATUS_ERR;
    }

    if (loop < 1) {
        return STATUS_ERR;
    } else if (loop == 1) {
        return STATUS_OK;    // 为1的时候，不设定即可
    }

    std::string session = "Loop-" + GraphNode::_generateSession();
    start->loop_ctrl_.setInfo(start, end, loop, session);
    end->loop_ctrl_.setInfo(start, end, loop, session);

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
        node->done_ = false;
        node->left_depend_ = node->dependence_.size();
    }

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::analyse() {
    CGRAPH_FUNCTION_BEGIN

    status = analyseCluster();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = analyseLoopRegion();
    CGRAPH_FUNCTION_CHECK_STATUS

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::analyseCluster() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    int runNodeSize = 0;
    int totalNodeSize = node_manage_->graph_nodes_.size();
    ClusterArr clusterVec;    // 每一层记录，可以并行的逻辑
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

        clusterVec.emplace_back(curCluster);
    }
    para_worked_clusters_.emplace_back(clusterVec);

    ClusterArr runnableClusters;
    while (!clusterVec.empty() && runNodeSize <= totalNodeSize) {
        runnableClusters = clusterVec;
        clusterVec.clear();

        for (GraphNodeCluster& cluster : runnableClusters) {
            /* 这一层没有将所有节点设置成done状态 */
            status = cluster.fakeProcess();
            CGRAPH_FUNCTION_CHECK_STATUS
        }
        runNodeSize += runnableClusters.size();

        std::set<GraphNode *> duplications;    // 防止当前流程中，单个节点重复添加，做去重使用
        for (GraphNodeCluster& cluster : runnableClusters) {
            /* 把执行过的节点取出来，查看他们的后继节点是否有可以运行的 */
            for (GraphNode* node : cluster.getNodes()) {
                for (GraphNode* cur : node->run_before_) {
                    /**
                     * 判断node是否需要被加入queue_中进行下一轮循环的条件
                     * 1，该node是可以执行的
                     * 2，该node本次循环是第一次被遍历
                     */
                    if (cur->isRunnable()
                        && duplications.find(cur) == duplications.end()) {

                        GraphNodeCluster curCluster;
                        GraphNode* curNode = cur;
                        curCluster.addNode(curNode);
                        duplications.insert(curNode);

                        while (curNode->isLinkable()
                               && 1 == curNode->run_before_.size()
                               && (*curNode->run_before_.begin())->isLinkable()) {
                            curNode = (*curNode->run_before_.begin());
                            curCluster.addNode(curNode);
                            duplications.insert(curNode);
                        }
                        clusterVec.emplace_back(curCluster);
                    }
                }
            }
        }

        /* 为空的话，直接退出循环；不为空的话，放入para信息中 */
        if (!clusterVec.empty()) {
            para_worked_clusters_.emplace_back(clusterVec);
        }
    }

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::analyseLoopRegion() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(false)

    std::string loopSession;
    bool inLoop = false;
    GraphNodeLoopRegion loopRegion;

    for (ClusterArr& clusterArr : para_worked_clusters_) {
        if (!inLoop && 1 == clusterArr.front().loop_) {
            /* 如果不在loop中，切当前cluArr的loop数为1，则 */
            loopRegion.clear();
            loopRegion.addClusterArr(clusterArr, false);
            this->loop_regions_.emplace_back(loopRegion);
            loopRegion.clear();
        } else if (!inLoop && 1 < clusterArr.front().loop_) {
            /* 如果不在loop中，切刚刚进入loop
             * cluster如果进入/退出loop阶段，肯定仅有一个node信息 */
            loopRegion.addClusterArr(clusterArr, true);
            inLoop = true;
            loopSession = clusterArr.front().getNodes().front()->loop_ctrl_.session_;
        } else if (inLoop) {
            /* 在loop中，直接添加。如果是loop结束，则设置标记位 */
            loopRegion.addClusterArr(clusterArr, false);
            if (!loopSession.empty()
                && loopSession == clusterArr.front().getNodes().front()->loop_ctrl_.session_) {
                this->loop_regions_.emplace_back(loopRegion);
                loopSession.clear();
                inLoop = false;
            }
        }
    }

    CGRAPH_FUNCTION_END
}
