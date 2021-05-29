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
    status = preCheck();
    CGRAPH_FUNCTION_CHECK_STATUS

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

/**
 * GraphNodeManager 不应该被执行run方法
 * @return
 */
CSTATUS GraphNodeManager::run() {
    CGRAPH_PROCESS_ERROR
}

CSTATUS GraphNodeManager::preCheck() {
    CGRAPH_FUNCTION_BEGIN

    /* 优先遍历一遍节点，判定哪些是可以连接成cluster的 */
    for (GraphNode* node : graph_nodes_) {
        /**
         * 认定图可以连通的判定条件：
         * 1，当前节点仅有一个依赖
         * 2，当前节点依赖的节点，只有一个后继
         * 3，当前节点的依赖的后继，仍是当前节点
         * 4，当前节点不是循环的开始/结尾节点
         */
        if (1 == node->dependence_.size()
            && 1 == (*node->dependence_.begin())->run_before_.size()
            && (*(node->dependence_.begin()))->run_before_.find(node) != (*(node->dependence_.begin()))->run_before_.end()
            && node->loop_ctrl_.enableLink()) {
            node->linkable_ = true;
        }
    }

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
