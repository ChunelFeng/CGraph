/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyLogAspect.h
@Time: 2021/9/29 10:51 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYLOGGERASPECT_H
#define CGRAPH_MYLOGGERASPECT_H

#include "../../src/CGraph.h"

template <typename T>
class MyLoggerAspect : public GAspect<T> {

public:
    void begin() override {
        CGRAPH_ECHO("----> begin to run this node ...");
    }

    void finish() override {
        CGRAPH_ECHO("----> run this node finished ...");
    }
};

#endif //CGRAPH_MYLOGGERASPECT_H
