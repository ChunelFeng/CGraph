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

namespace py = pybind11;

class PywGAspect : public CGraph::GAspect {
public:
    explicit PywGAspect() {};
    ~PywGAspect() override {};

    CStatus beginInit() override {
        PYBIND11_OVERLOAD(CStatus, GAspect, beginInit);
    }

    CVoid finishInit(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, GAspect, finishInit, curStatus);
    }

    CStatus beginRun() override {
        PYBIND11_OVERLOAD(CStatus, GAspect, beginRun);
    }

    CVoid finishRun(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, GAspect, finishRun, curStatus);
    }

    CStatus beginDestroy() override {
        PYBIND11_OVERLOAD(CStatus, GAspect, beginDestroy);
    }

    CVoid finishDestroy(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, GAspect, finishDestroy, curStatus);
    }

    CVoid enterCrashed() override {
        PYBIND11_OVERLOAD(CVoid, GAspect, enterCrashed);
    }
};

#endif //CGRAPH_PYWGASPECT_H
