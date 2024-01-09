/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: TestGNodes.h
@Time: 2023/12/3 17:40
@Desc: 
***************************/

#ifndef CGRAPH_TESTGNODES_H
#define CGRAPH_TESTGNODES_H

#include <memory>

#include "TestCommonDefine.h"
#include "TestGParams.h"

class TestAdd1GNode : public CGraph::GNode {
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
        if (0 != g_test_node_cnt % 1000 || 0 == g_test_node_cnt) {
            status.setErrorInfo("test node count is " + std::to_string(g_test_node_cnt.load()));
        }
        return status;
    }
};


class TestReturnErrorGNode : public CGraph::GNode {
public:
    CStatus run() override {
        return CStatus("test error return, no real problem");
    }
};


class TestRecvMessageGNode : public CGraph::GNode {
public:
    CStatus run() override {
        std::unique_ptr<TestGMessageParam> mp = nullptr;
        CStatus status = CGRAPH_RECV_MPARAM_WITH_TIMEOUT(TestGMessageParam, g_test_message_key, mp, 200);
        if (status.isErr()) {
            CGraph::CGRAPH_ECHO("error message is [%s]",  status.getInfo().c_str());
            return status;
        }

        mp.get()->num_++;
        CGRAPH_SEND_MPARAM(TestGMessageParam, g_test_message_key, mp, CGraph::GMessagePushStrategy::WAIT)
        return status;
    }
};

#endif //CGRAPH_TESTGNODES_H
