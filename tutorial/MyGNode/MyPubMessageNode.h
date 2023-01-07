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
#include "../MyParams/MyMessageParam.h"

class MyPubMessageNode : public CGraph::GNode {
public:
    CStatus init() override {
        int sleep = 300;
        CGRAPH_SLEEP_MILLISECOND(sleep)    // 休息，确保其他的sub节点，已经完成了订阅
        CGraph::CGRAPH_ECHO("==> [MyPubMessageNode] [init], sleep [%d] ms, to make sure sub node prepare ok.",
                            sleep);
        return CStatus();
    }

    CStatus run() override {
        MyMessageParam mp;    // 创建一个消息，并且发送出去
        mp.num = (num_++) * 100;
        mp.info = "this is a test info, num = " + std::to_string(mp.num);
        CStatus status = CGRAPH_PUB_MPARAM(MyMessageParam, "pub-sub", mp);
        return status;
    }

private:
    int num_ = 0;
};


#endif //CGRAPH_MYPUBMESSAGENODE_H
