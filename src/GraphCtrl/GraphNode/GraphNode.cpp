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
    run_before_.clear();
    dependence_.clear();
}

GraphNode::~GraphNode() {
    done_.store(true);
    run_before_.clear();
    left_depend_ = 0;
    dependence_.clear();
};

GraphNode::GraphNode(const GraphNode& node) {
    this->done_ = node.done_.load();
    for (GraphNode* cur : node.run_before_) {
        this->run_before_.insert(cur);
    }

    this->run_before_ = node.run_before_;
    this->dependence_ = node.dependence_;

    this->left_depend_ = node.left_depend_;
}

CSTATUS GraphNode::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_FUNCTION_END
}

CSTATUS GraphNode::deinit() {
    CGRAPH_FUNCTION_BEGIN
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
    this->done_.store(false);
    this->left_depend_ = this->dependence_.size();
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

