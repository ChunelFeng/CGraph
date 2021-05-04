/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphNode.cpp
@Time: 2021/4/26 9:29 下午
@Desc: 
***************************/

#include "GraphNode.h"


GraphNode::GraphNode() {
    left_depend_ = 0;
    done_.store(false);
    dependence_.clear();
    run_before_.clear();
}

GraphNode::~GraphNode() {
    done_.store(true);
    dependence_.clear();
    run_before_.clear();
    left_depend_ = 0;
};

CSTATUS GraphNode::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_FUNCTION_END
}

CSTATUS GraphNode::deinit() {
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
    node->run_before_.push_back(this);
    this->left_depend_++;
    this->dependence_.push_back(node);

    CGRAPH_FUNCTION_END
}

/**
 * 当前节点，所有依赖均执行完毕，切未被执行的时候，则可以执行
 * @return
 */
bool GraphNode::isRunnable() const {
    return 0 >= this->left_depend_ && false == this->done_.load();
}

CSTATUS GraphNode::beforeRun() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_FUNCTION_END
}

CSTATUS GraphNode::afterRun() {
    CGRAPH_FUNCTION_BEGIN

    for (GraphNode* node : this->run_before_) {
        node->left_depend_--;
    }

    this->done_.store(true);
    CGRAPH_FUNCTION_END
}

/**
 * 线程池执行本函数，包含了 beforeRun，run和afterRun功能
 * @return
 */
CSTATUS GraphNode::process() {
    CGRAPH_FUNCTION_BEGIN

    status = beforeRun();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = run();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = afterRun();
    CGRAPH_FUNCTION_END
}

