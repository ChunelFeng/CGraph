/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyNode2.h
@Time: 2021/5/4 4:09 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYNODE2_H
#define CGRAPH_MYNODE2_H

#include "../../src/CGraph.h"

class MyNode2 : public CGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        CGraph::CGRAPH_ECHO("[%s], enter MyNode2 run function. Sleep for 2 second ... ", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(2)
        return status;
    }
};

#endif //CGRAPH_MYNODE2_H
