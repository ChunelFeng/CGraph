/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGConditiion.h
@Time: 2025/1/30 21:38
@Desc: 
***************************/

#ifndef CGRAPH_PYWGCONDITION_H
#define CGRAPH_PYWGCONDITION_H

#include <pybind11/pybind11.h>

#include "CGraph.h"
#include "PyWrapperMacro.h"

class PywGCondition : public CGraph::GCondition {
public:
    explicit PywGCondition(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) {
        __addGElements_4py(elements);
    }
    ~PywGCondition() override = default;

    CIndex choose() override {
        PYBIND11_OVERLOAD_PURE(CIndex, PywGCondition, choose);
    }
};

PYCGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGCondition,
    CIndex choose() override {
        PYBIND11_OVERLOAD_PURE(CIndex, PywGConditionInterface, choose);
    });

#endif //CGRAPH_PYWGCONDITION_H
