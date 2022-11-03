/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyPubMessageNode.h
@Time: 2022/11/4 00:02
@Desc: 
***************************/

#ifndef CGRAPH_MYPUBMESSAGENODE_H
#define CGRAPH_MYPUBMESSAGENODE_H

#include "../../src/CGraph.h"
#include "../MyGParam/MyMessageParam.h"

class MyPubMessageNode : public CGraph::GNode {
public:
    CStatus run () override {
        MyMessageParam mp;    // 创建一个消息，并且发送出去
        mp.num = num_++;
        mp.info = "this is a test info";
        CStatus status = CGRAPH_PUB_MPARAM(MyMessageParam, "test", mp);
        return status;
    }

private:
    int num_ = 0;
};


#endif //CGRAPH_MYPUBMESSAGENODE_H
