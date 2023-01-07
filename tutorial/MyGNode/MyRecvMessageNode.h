/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyRecvMessageNode.h
@Time: 2022/12/27 00:11
@Desc: 
***************************/

#ifndef CGRAPH_MYRECVMESSAGENODE_H
#define CGRAPH_MYRECVMESSAGENODE_H

#include "../../src/CGraph.h"
#include "../MyParams/MyMessageParam.h"

class MyRecvMessageNode : public CGraph::GNode {
public:
    CStatus run() override {
        MyMessageParam mp;    // 接收一个消息
        CStatus status = CGRAPH_RECV_MPARAM(MyMessageParam, "send-recv", mp);
        if (!status.isOK()) {
            CGraph::CGRAPH_ECHO("MySubMessageNode sub message error");
            return status;
        }

        CGraph::CGRAPH_ECHO("num = [%d], info = [%s]", mp.num, mp.info.c_str());
        return status;
    }
};

#endif //CGRAPH_MYRECVMESSAGENODE_H
