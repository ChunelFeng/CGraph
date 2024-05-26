/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: HelloCGraphNode.h
@Time: 2021/6/24 1:06 上午
@Desc: 
***************************/

#ifndef CGRAPH_HELLOCGRAPHNODE_H
#define CGRAPH_HELLOCGRAPHNODE_H

#include "../../src/CGraph.h"

class HelloCGraphNode : public CGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        std::cout << "Hello, CGraph." << std::endl;
        return status;
    }
};

#endif //CGRAPH_HELLOCGRAPHNODE_H
