/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyShowAddressNode.h
@Time: 2021/10/31 12:50 上午
@Desc: 
***************************/

#ifndef CGRAPH_MYSHOWADDRESSNODE_H
#define CGRAPH_MYSHOWADDRESSNODE_H

#include "../../src/CGraph.h"
#include "../MyParams/MyParam.h"

class MyShowAddressNode : public CGraph::GNode {
public:
    CStatus init() override {
        CStatus status = CGRAPH_CREATE_GPARAM(MyParam, "param2")
        return status;
    }

    CStatus run() override {
        auto myParam = CGRAPH_GET_GPARAM(MyParam, "param2")
        if (nullptr == myParam) {
            return CStatus("get param2 failed");
        }

        int cnt = 0;
        {
            CGRAPH_PARAM_WRITE_CODE_BLOCK(myParam)
            cnt = ++myParam->iCount;
        }

        CGraph::CGRAPH_ECHO("singleton node run, name is [%s], this address is [0x%x], cnt is [%d].",
                            this->getName().c_str(), (unsigned int*)this, cnt);
        return CStatus();
    }
};

#endif //CGRAPH_MYSHOWADDRESSNODE_H
