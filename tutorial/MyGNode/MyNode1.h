/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CMyGraphNode1.h
@Time: 2021/5/4 3:57 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYNODE1_H
#define CGRAPH_MYNODE1_H

#include "../../src/CGraph.h"

class MyNode1 : public CGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        CGraph::CGRAPH_ECHO("[%s], enter MyNode1 run function. Sleep for 1 second ... ", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(1)
        return status;
    }
};

#endif //CGRAPH_MYNODE1_H
