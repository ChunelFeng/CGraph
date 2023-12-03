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

std::atomic<unsigned int> g_test_cnt = {0};
class TestMaterialAdd1GNode : public CGraph::GNode {
public:
    CStatus run() override {
        g_test_cnt++;
        return CStatus();
    }
};

#endif //CGRAPH_TESTGNODES_H
