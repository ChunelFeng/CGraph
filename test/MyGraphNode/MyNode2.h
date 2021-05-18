/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyNode2.h
@Time: 2021/5/4 4:09 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYNODE2_H
#define CGRAPH_MYNODE2_H

#include "../../src/GraphCtrl/GraphInclude.h"

class MyNode2 : public GraphNode {

public:
    CSTATUS run () override {
        CSTATUS status = STATUS_OK;
        std::cout << this->getName() << ", enter MyNode2 run function. sleep for 2 second ... " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        return status;
    }
};


#endif //CGRAPH_MYNODE2_H
