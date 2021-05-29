/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphNodeCluster.cpp
@Time: 2021/5/8 10:59 下午
@Desc: 
***************************/

#include "GraphNodeCluster.h"

GraphNodeCluster::GraphNodeCluster() {
    this->loop_ = 1;
}

GraphNodeCluster::~GraphNodeCluster() {
    this->loop_ = 1;
    this->node_arr_.clear();
}

GraphNodeCluster::GraphNodeCluster(const GraphNodeCluster& cluster) {
    this->node_arr_ = cluster.node_arr_;
    this->loop_ = cluster.loop_;
}

GraphNodeCluster& GraphNodeCluster::operator=(const GraphNodeCluster& cluster) {
    if (this == &cluster) {
        return *this;
    }

    this->node_arr_ = cluster.node_arr_;
    this->loop_ = cluster.loop_;
    return *this;
}

CSTATUS GraphNodeCluster::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_FUNCTION_END
}

CSTATUS GraphNodeCluster::deinit() {
    CGRAPH_FUNCTION_BEGIN
    node_arr_.clear();
    CGRAPH_FUNCTION_END
}

CSTATUS GraphNodeCluster::run() {
    CGRAPH_PROCESS_ERROR
}

CSTATUS GraphNodeCluster::process() {
    CGRAPH_FUNCTION_BEGIN
    for (GraphNode* node : node_arr_) {
        CGRAPH_NODE_SAFE_PROCESS(node)
    }

    CGRAPH_FUNCTION_END
}

CSTATUS GraphNodeCluster::addNode(GraphNode* node) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(node)

    node_arr_.emplace_back(node);
    loop_ = node->loop_ctrl_.loop_;    // 当loop不为1的node，不会进行cluster合并

    CGRAPH_FUNCTION_END
}

void GraphNodeCluster::clear() {
    node_arr_.clear();
}

int GraphNodeCluster::size() const {
    return node_arr_.size();
}

const GNodeArr& GraphNodeCluster::getNodes() const {
    return this->node_arr_;
}

CSTATUS GraphNodeCluster::fakeProcess() {
    CGRAPH_FUNCTION_BEGIN

    for (GraphNode* node : node_arr_) {
        status = node->beforeRun();
        CGRAPH_FUNCTION_CHECK_STATUS
        /* 不执行run方法，模拟运行 */
        status = node->afterRun();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}
