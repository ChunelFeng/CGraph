/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphNodeManager.cpp
@Time: 2021/5/6 10:38 下午
@Desc: 
***************************/

#include "GraphNodeManager.h"

GraphNodeManager::GraphNodeManager() {
}

GraphNodeManager::~GraphNodeManager() {
    // 删除所有节点信息
    std::for_each(graph_nodes_.begin(), graph_nodes_.end(),
                  [](GraphNode* node){CGRAPH_DELETE_PTR(node)});
}

CSTATUS GraphNodeManager::init() {
    CGRAPH_FUNCTION_BEGIN
    for (GraphNode* node : graph_nodes_) {
        CGRAPH_ASSERT_NOT_NULL(node)
        status = node->init();
        CGRAPH_FUNCTION_CHECK_STATUS
    }
    CGRAPH_FUNCTION_END
}

CSTATUS GraphNodeManager::deinit() {
    CGRAPH_FUNCTION_BEGIN
    std::for_each(graph_nodes_.begin(), graph_nodes_.end(),
                  [](GraphNode* node){node->deinit();});
    this->graph_nodes_.clear();
    CGRAPH_FUNCTION_END
}

CSTATUS GraphNodeManager::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_FUNCTION_END
}

CSTATUS GraphNodeManager::insertNode(GraphNode* node) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(node)

    this->graph_nodes_.insert(node);

    CGRAPH_FUNCTION_END
}

bool GraphNodeManager::hasNode(GraphNode* node) const {
    if (node == nullptr) {
        return false;
    }

    return graph_nodes_.find(node) != graph_nodes_.end();
}

void GraphNodeManager::deleteNode(GraphNode* node) {
    graph_nodes_.erase(node);
}

CSTATUS GraphNodeManager::multiProcess(const GNodeArr& nodes) {
    CGRAPH_FUNCTION_BEGIN
    for (GraphNode* node : nodes) {
        CGRAPH_ASSERT_NOT_NULL(node)
        status = node->process();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}