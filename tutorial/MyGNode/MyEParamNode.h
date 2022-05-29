/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyEParamNode.h
@Time: 2022/5/29 22:37
@Desc: 
***************************/

#ifndef CGRAPH_MYEPARAMNODE_H
#define CGRAPH_MYEPARAMNODE_H

#include "../../src/CGraph.h"
#include "../MyGParam/MyVersionParam.h"
#include "../MyGParam/MyConnParam.h"

class MyEParamNode : public CGraph::GNode {
public:
    CStatus run() override {
        // 获取本地参数信息，不同的node具体的值，根据外部注册进来的不同，而不同
        auto* version = this->getEParam<MyVersionParam>(VERSION_PARAM_KEY);
        if (nullptr != version) {
            CGraph::CGRAPH_ECHO("[%s] version is [%d-%d]",
                                this->getName().c_str(), version->priority_, version->secondary_);
        }

        auto* conn = this->getEParam<MyConnParam>(CONN_PARAM_KEY);
        if (nullptr != conn) {
            CGraph::CGRAPH_ECHO("[%s], ip = [%s], port = [%u]",
                                this->getName().c_str(), conn->ip_.c_str(), conn->port_);
        }

        return CStatus();
    }
};

#endif //CGRAPH_MYEPARAMNODE_H
