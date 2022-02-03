/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyDaemon.h
@Time: 2022/2/3 12:08 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYDAEMON_H
#define CGRAPH_MYDAEMON_H

#include "../../src/CGraph.h"

class MyDaemon : public CGraph::GDaemon {
    CMSec getInterval() override {
        return 1500;
    }

    CVoid daemonRun() override {
        CGraph::CGRAPH_ECHO("hello, daemon");
    }
};

#endif //CGRAPH_MYDAEMON_H
