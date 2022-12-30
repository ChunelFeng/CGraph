/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyMonitorDaemon.h
@Time: 2022/2/3 12:08 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYMONITORDAEMON_H
#define CGRAPH_MYMONITORDAEMON_H

#include "../../src/CGraph.h"

class MyMonitorDaemon : public CGraph::GDaemon {
public:
    CVoid daemonTask(CGraph::GDaemonParamPtr param) override {
        CGraph::CGRAPH_ECHO("----> [MyMonitorDaemon] this monitor is still running, monitor span is [%d] ms...",
                            getInterval());
    }
};

#endif //CGRAPH_MYMONITORDAEMON_H
