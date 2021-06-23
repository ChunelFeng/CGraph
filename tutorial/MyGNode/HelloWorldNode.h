/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: HelloWorldNode.h
@Time: 2021/6/24 1:06 上午
@Desc: 
***************************/

#ifndef CGRAPH_HELLOWORLDNODE_H
#define CGRAPH_HELLOWORLDNODE_H

#include "../../src/GraphCtrl/GraphInclude.h"

class HelloWorldNode : public GNode {

public:
    CSTATUS run () override {
        std::cout << "hello, world." << std::endl;
        return STATUS_OK;
    }
};

#endif //CGRAPH_HELLOWORLDNODE_H
