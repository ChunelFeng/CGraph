/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: HelloWorldNode.h
@Time: 2021/6/24 1:06 上午
@Desc: 
***************************/

#ifndef CGRAPH_HELLOWORLDNODE_H
#define CGRAPH_HELLOWORLDNODE_H

#include "../../src/CGraph.h"

class HelloWorldNode : public CGraph::GNode {

public:
    CStatus run () override {
        CStatus status;
        std::cout << "hello, world." << std::endl;
        return status;
    }
};

#endif //CGRAPH_HELLOWORLDNODE_H
