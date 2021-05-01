/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphNode.cpp
@Time: 2021/4/26 9:29 下午
@Desc: 
***************************/

#include "GraphNode.h"

GraphNode::GraphNode() {
    this->left_depend_ = 0;
    this->done_ = false;
}

GraphNode::~GraphNode() = default;

CSTATUS GraphNode::init() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_FUNCTION_END
}

CSTATUS GraphNode::deinit() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_FUNCTION_END
}

CSTATUS GraphNode::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_FUNCTION_END
}

/**
 * 添加相互依赖关系。本节点依赖于node节点
 * @param node
 * @return
 */
CSTATUS GraphNode::addDependNode(GraphNode* node) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(node)

    // node依赖this的执行，this节点需要在node之前运行
    this->addDependence(node);
    node->addRunBefore(this);

    CGRAPH_FUNCTION_END
}

std::list<GraphNode *>& GraphNode::getRunBeforeList(){
    return this->run_before_;
}

CSTATUS GraphNode::addRunBefore(GraphNode* node) {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_NOT_NULL(node)
    this->run_before_.push_back(node);
    node->left_depend_++;

    CGRAPH_FUNCTION_END
}

CSTATUS GraphNode::addDependence(GraphNode* node) {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_NOT_NULL(node)
    this->dependence_.push_back(node);
    CGRAPH_FUNCTION_END
}

/**
 * 判断是否可以执行
 * @return
 */
bool GraphNode::enableRun() const {
    return this->left_depend_ == 0;
}

bool GraphNode::isDone() const {
    return this->done_;
}

CSTATUS GraphNode::afterRun() {
    CGRAPH_FUNCTION_BEGIN

    for (GraphNode* node : this->run_before_) {
        node->left_depend_--;
    }

    this->done_ = true;
    CGRAPH_FUNCTION_END
}

