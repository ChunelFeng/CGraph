/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CMyGraphNode1.h
@Time: 2021/5/4 3:57 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYNODE1_H
#define CGRAPH_MYNODE1_H

#include <iostream>

#include "../../src/GraphCtrl/GraphInclude.h"

class MyNode1 : public GraphNode {

public:
    CSTATUS run () override {
        int status = STATUS_OK;
        std::cout << "enter node1 run function. sleep for 1 second ... " << std::endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
        return status;
    }
};

#endif //CGRAPH_NODE1_H
