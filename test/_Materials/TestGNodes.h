/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: TestGNodes.h
@Time: 2023/12/3 17:40
@Desc: 
***************************/

#ifndef CGRAPH_TESTGNODES_H
#define CGRAPH_TESTGNODES_H

#include <atomic>

#include "CGraph.h"

std::atomic<unsigned int> g_test_node_cnt = {0};
class TestMaterialAdd1GNode : public CGraph::GNode {
public:
    CStatus init() override {
        g_test_node_cnt = 0;
        return CStatus();
    }

    CStatus run() override {
        g_test_node_cnt++;
        return CStatus();
    }

    CStatus destroy() override {
        CStatus status;
        if (0 != g_test_node_cnt % 10000) {
            status.setErrorInfo("test node count is " + std::to_string(g_test_node_cnt.load()));
        }
        return status;
    }
};

#endif //CGRAPH_TESTGNODES_H
