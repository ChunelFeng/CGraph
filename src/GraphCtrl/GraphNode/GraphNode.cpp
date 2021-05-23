/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphNode.cpp
@Time: 2021/4/26 9:29 下午
@Desc: 
***************************/

#include <uuid/uuid.h>
#include "GraphNode.h"


GraphNode::GraphNode() {
    left_depend_ = 0;
    done_ = false;
    run_before_.clear();
    dependence_.clear();
    generateSession();
}

GraphNode::~GraphNode() {
    done_ = true;
    run_before_.clear();
    left_depend_ = 0;
    dependence_.clear();
};

GraphNode::GraphNode(const GraphNode& node) {
    this->done_ = node.done_;
    for (GraphNode* cur : node.run_before_) {
        this->run_before_.emplace(cur);
    }

    this->dependence_ = node.dependence_;
    this->left_depend_.store(node.left_depend_);
    this->linkable_ = node.linkable_;
    this->session_ = node.session_;
    this->loop_ = node.loop_;
    this->name_ = node.name_;
}

GraphNode& GraphNode::operator=(const GraphNode& node) {
    if (this == &node) {
        return *this;
    }

    this->done_ = node.done_;
    for (GraphNode* cur : node.run_before_) {
        this->run_before_.emplace(cur);
    }

    this->dependence_ = node.dependence_;
    this->left_depend_.store(node.left_depend_);
    this->linkable_ = node.linkable_;
    this->session_ = node.session_;
    this->loop_ = node.loop_;
    this->name_ = node.name_;

    return *this;
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
    return 0 >= this->left_depend_ && false == this->done_;
}

bool GraphNode::isLinkable() const {
    return this->linkable_;
}

CSTATUS GraphNode::beforeRun() {
    CGRAPH_FUNCTION_BEGIN
    this->done_ = false;
    this->left_depend_ = this->dependence_.size();
    CGRAPH_FUNCTION_END
}

CSTATUS GraphNode::afterRun() {
    CGRAPH_FUNCTION_BEGIN

    for (GraphNode* node : this->run_before_) {
        node->left_depend_--;
    }

    this->done_ = true;
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

    for (int i = 0; i < this->loop_; i++) {
        status = run();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    status = afterRun();
    CGRAPH_FUNCTION_END
}

void GraphNode::setName(const std::string& name) {
    if (likely(!name.empty())) {
        this->name_ = name;
    } else {
        this->name_ = this->session_;
    }
}

void GraphNode::setLoop(int loop) {
    this->loop_ = loop;
}

void GraphNode::generateSession() {
    uuid_t uuid;
    char session[36] = {0};    // 36是特定值
    uuid_generate(uuid);
    uuid_unparse(uuid, session);
    this->session_ = session;
}

const std::string& GraphNode::getName() const {
    return this->name_;
}

const std::string& GraphNode::getSession() const {
    return this->session_;
}
