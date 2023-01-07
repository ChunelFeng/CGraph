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
#include "../MyParams/MyMessageParam.h"

template <typename ...Args>
class MySubMessageNode : public CGraph::GTemplateNode<Args ...> {
public:
    explicit MySubMessageNode(int connId) {
        conn_id_ = connId;
    }

    CStatus init() override {
        CGraph::CGRAPH_ECHO("==> [%s] [init] get conn id = [%d]",
                            this->getName().c_str(), conn_id_);
        return CStatus();
    }

    CStatus run() override {
        MyMessageParam mp;    // 接收一个消息
        CStatus status = CGRAPH_SUB_MPARAM(MyMessageParam, conn_id_, mp)
        if (!status.isOK()) {
            CGraph::CGRAPH_ECHO("[%s] recv message error", this->getName().c_str());
            return status;
        }

        CGraph::CGRAPH_ECHO("[%s] conn id = [%d], num = [%d], info = [%s]",
                            this->getName().c_str(), conn_id_, mp.num, mp.info.c_str());

        return status;
    }

private:
    CIndex conn_id_;
};

#endif //CGRAPH_MYSUBMESSAGENODE_H
