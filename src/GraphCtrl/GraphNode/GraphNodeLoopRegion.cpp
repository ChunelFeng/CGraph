/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphLoop.cpp
@Time: 2021/5/28 10:23 下午
@Desc:
***************************/

#include "GraphNodeLoopRegion.h"

GraphNodeLoopRegion::GraphNodeLoopRegion() {
    loop_ = 1;
}

GraphNodeLoopRegion::~GraphNodeLoopRegion() {
    this->para_worked_clusters_.clear();
}

GraphNodeLoopRegion::GraphNodeLoopRegion(const GraphNodeLoopRegion& loopRegion) {
    this->loop_ = loopRegion.loop_;
    this->para_worked_clusters_ = loopRegion.para_worked_clusters_;
}

GraphNodeLoopRegion& GraphNodeLoopRegion::operator=(const GraphNodeLoopRegion& loopRegion) {
    if (&loopRegion == this) {
        return *this;
    }

    this->loop_ = loopRegion.loop_;
    this->para_worked_clusters_ = loopRegion.para_worked_clusters_;

    return *this;
}

CSTATUS GraphNodeLoopRegion::addClusterArr(std::vector<GraphNodeCluster>& clusterArr, bool isHead) {
    CGRAPH_FUNCTION_BEGIN
    if (clusterArr.empty()) {
        return STATUS_ERR;
    }

    this->para_worked_clusters_.emplace_back(clusterArr);
    if (isHead) {
        this->loop_ = clusterArr[0].loop_;
    }

    CGRAPH_FUNCTION_END
}

CSTATUS GraphNodeLoopRegion::run() {
    CGRAPH_PROCESS_ERROR
}

void GraphNodeLoopRegion::clear() {
    this->loop_ = 1;
    this->para_worked_clusters_.clear();
}
