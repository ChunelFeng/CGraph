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
#include "PyWrapperMacro.h"

class PywGParam : public CGraph::GParam {
public:
    ~PywGParam() override = default;

protected:
    CStatus init() override {
        PYCGRAPH_OVERRIDE_RET_CSTATUS(CGraph::GParam, init);
    }

    CStatus destroy() override {
        PYCGRAPH_OVERRIDE_RET_CSTATUS(CGraph::GParam, destroy);
    }

    CStatus setup() override {
        PYCGRAPH_OVERRIDE_RET_CSTATUS(CGraph::GParam, setup);
    }

    CVoid reset(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, CGraph::GParam, reset, curStatus);
    }
};

#endif //CGRAPH_PYWGPARAM_H
