/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGDaemon.h
@Time: 2025/3/30 00:13
@Desc: 
***************************/

#ifndef CGRAPH_PYWGDAEMON_H
#define CGRAPH_PYWGDAEMON_H

#include "CGraph.h"

class PywGDaemon : public CGraph::GDaemon {
public:
    explicit PywGDaemon() = default;
    ~PywGDaemon() override = default;

protected:
    CVoid daemonTask(CGraph::GDaemonParamPtr param) override {
        PYBIND11_OVERLOAD_PURE(CVoid, GDaemon, daemonTask, param);
    }

    CMSec modify(CGraph::GDaemonParamPtr param) override {
        PYBIND11_OVERLOAD(CMSec, GDaemon, modify, param);
    }
};

#endif //CGRAPH_PYWGDAEMON_H
