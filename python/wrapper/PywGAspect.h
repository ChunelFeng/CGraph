/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGAspect.h
@Time: 2025/3/5 23:06
@Desc: 
***************************/

#ifndef CGRAPH_PYWGASPECT_H
#define CGRAPH_PYWGASPECT_H

#include <pybind11/pybind11.h>

#include "CGraph.h"
#include "PyWrapperMacro.h"

class PywGAspect : public CGraph::GAspect {
public:
    explicit PywGAspect() = default;
    ~PywGAspect() override = default;

protected:
    CStatus beginInit() override {
        PYCGRAPH_OVERRIDE_RET_CSTATUS(GAspect, beginInit);
    }

    CVoid finishInit(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, GAspect, finishInit, curStatus);
    }

    CStatus beginRun() override {
        PYCGRAPH_OVERRIDE_RET_CSTATUS(GAspect, beginRun);
    }

    CVoid finishRun(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, GAspect, finishRun, curStatus);
    }

    CStatus beginDestroy() override {
        PYCGRAPH_OVERRIDE_RET_CSTATUS(GAspect, beginDestroy);
    }

    CVoid finishDestroy(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, GAspect, finishDestroy, curStatus);
    }

    CVoid enterCrashed() override {
        PYBIND11_OVERLOAD(CVoid, GAspect, enterCrashed);
    }

    CVoid enterTimeout() override {
        PYBIND11_OVERLOAD(CVoid, GAspect, enterTimeout);
    }
};

#endif //CGRAPH_PYWGASPECT_H
