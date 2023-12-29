/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: TestGAspects.h
@Time: 2023/12/29 21:59
@Desc: 
***************************/

#ifndef CGRAPH_TESTGASPECTS_H
#define CGRAPH_TESTGASPECTS_H

#include "TestCommonDefine.h"

class TestMaterialAdd1GAspect : public CGraph::GAspect {
public:
    CStatus beginRun() override {
        g_test_node_cnt++;
        return CStatus();
    }

    CVoid finishRun(const CStatus& curStatus) override {
        g_test_node_cnt++;
    }
};


#endif //CGRAPH_TESTGASPECTS_H
