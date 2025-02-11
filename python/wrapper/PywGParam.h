/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGParam.h
@Time: 2025/2/1 14:04
@Desc: 
***************************/

#ifndef CGRAPH_PYWGPARAM_H
#define CGRAPH_PYWGPARAM_H

#include <pybind11/pybind11.h>

#include "CGraph.h"

namespace py = pybind11;

class PywGParam : public CGraph::GParam {
protected:
    CStatus init() override {
        PYBIND11_OVERLOAD(CStatus, CGraph::GParam, init);
    }

    CStatus destroy() override {
        PYBIND11_OVERLOAD(CStatus, CGraph::GParam, destroy);
    }

    CStatus setup() override {
        PYBIND11_OVERLOAD(CStatus, CGraph::GParam, setup);
    }

    CVoid reset(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, CGraph::GParam, reset, curStatus);
    }
};

#endif //CGRAPH_PYWGPARAM_H
