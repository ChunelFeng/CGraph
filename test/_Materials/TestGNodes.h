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
};


class TestReturnErrorGNode : public CGraph::GNode {
public:
    CStatus run() override {
        return CStatus("test error return, no real problem");
    }
};

class TestAdd1ByParamGNode : public CGraph::GNode {
public:
    CStatus run() override {
        auto p = getGParamWithNoEmpty<TestAdd1GParam>(g_add1_by_param_key);
        {
            CGRAPH_PARAM_WRITE_CODE_BLOCK(p)
            p->cnt_++;
        }
        return CStatus();
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
