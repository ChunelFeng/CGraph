/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphNodeCluster.cpp
@Time: 2021/5/8 10:59 下午
@Desc: 
***************************/

#include "GraphNodeCluster.h"

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

CSTATUS GraphNodeCluster::multiProcess() {
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

    CGRAPH_FUNCTION_END
}

void GraphNodeCluster::clear() {
    node_arr_.clear();
}

int GraphNodeCluster::size() const {
    return (int)node_arr_.size();
}

GNodeArr& GraphNodeCluster::getNodes() {
    return this->node_arr_;
}
