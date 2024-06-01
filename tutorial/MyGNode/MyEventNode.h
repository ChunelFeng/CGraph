/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyEventNode.h
@Time: 2023/1/20 18:31
@Desc: 
***************************/

#ifndef CGRAPH_MYEVENTNODE_H
#define CGRAPH_MYEVENTNODE_H

#include "CGraph.h"

class MyEventNode : public CGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        CGraph::CGRAPH_ECHO("[%s], before event notify", this->getName().c_str());
        /**
         * 模拟在这里，触发一个 trigger信息，同名的事件被异步执行
         * 从打印结果可以看出，after event send 这条信息，提前执行
         * 执行的时候，和pipeline公用同一个线程池资源
         */
        notify("my-print-event", GEventType::SYNC);

        CGraph::CGRAPH_ECHO("[%s], after event notify", this->getName().c_str());
        return status;
    }
};

#endif //CGRAPH_MYEVENTNODE_H
