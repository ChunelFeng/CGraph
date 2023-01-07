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
#include "../MyParams/MyVersionParam.h"
#include "../MyParams/MyConnParam.h"

class MyEParamNode : public CGraph::GNode {
public:
    CStatus run() override {
        /**
         * 获取本地参数信息，不同的node具体的值，根据外部注册进来的不同，而不同
         * 注：这里跟 CGRAPH_GET_GPARAM 的区别是：
         * 1，通过 CGRAPH_GET_EPARAM 获取的参数，是在非节点内部生成的，仅能在当前节点获取和使用，且类型是 GElementParam 类的子类
         * 2，通过 CGRAPH_GET_GPARAM 获取的参数，是任意节点内部生成，可以在pipeline中传递的，且类型是 GParam 类的子类
         */
        auto* version = CGRAPH_GET_EPARAM(MyVersionParam, VERSION_PARAM_KEY)
        if (nullptr != version) {
            CGraph::CGRAPH_ECHO("[%s] version is [%d-%d]",
                                this->getName().c_str(), version->priority_, version->secondary_);
        }

        auto* conn = CGRAPH_GET_EPARAM(MyConnParam, CONN_PARAM_KEY);
        if (nullptr != conn) {
            CGraph::CGRAPH_ECHO("[%s] ip = [%s], port = [%u]",
                                this->getName().c_str(), conn->ip_.c_str(), conn->port_);
        }

        return CStatus();
    }
};

#endif //CGRAPH_MYEPARAMNODE_H
