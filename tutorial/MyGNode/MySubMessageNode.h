/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MySubMessageNode.h
@Time: 2022/11/4 00:02
@Desc: 
***************************/

#ifndef CGRAPH_MYSUBMESSAGENODE_H
#define CGRAPH_MYSUBMESSAGENODE_H

#include "../../src/CGraph.h"
#include "../MyGParam/MyMessageParam.h"


class MySubMessageNode : public CGraph::GNode {
public:
    CStatus run () override {
        MyMessageParam mp;    // 接收一个消息
        CStatus status = CGRAPH_SUB_MPARAM(MyMessageParam, "test", mp);
        if (!status.isOK()) {
            CGraph::CGRAPH_ECHO("MySubMessageNode sub message error");
            return status;
        }

        CGraph::CGRAPH_ECHO("num = [%d], info = [%s]", mp.num, mp.info.c_str());
        return status;
    }
};

#endif //CGRAPH_MYSUBMESSAGENODE_H
