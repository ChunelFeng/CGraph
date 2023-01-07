/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MySendMessageNode.h
@Time: 2022/12/27 00:05
@Desc: 
***************************/

#ifndef CGRAPH_MYSENDMESSAGENODE_H
#define CGRAPH_MYSENDMESSAGENODE_H

#include "../../src/CGraph.h"
#include "../MyParams/MyMessageParam.h"

class MySendMessageNode : public CGraph::GNode {
public:
    CStatus run() override {
        MyMessageParam mp;    // 创建一个消息，并且发送出去
        mp.num = (num_++) * 10;
        mp.info = "this is a test send info, num = " + std::to_string(mp.num);
        CStatus status = CGRAPH_SEND_MPARAM(MyMessageParam, "send-recv", mp);
        return status;
    }

private:
    int num_ = 0;
};


#endif //CGRAPH_MYSENDMESSAGENODE_H
