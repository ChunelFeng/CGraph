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

namespace py = pybind11;

class PywGCondition : public CGraph::GCondition {
public:
    explicit PywGCondition(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) {
        __addElements_4py(elements);
    }
    ~PywGCondition() override {};

    CIndex choose() override {
        PYBIND11_OVERLOAD_PURE(CIndex, PywGCondition, choose);
    }
};

#endif //CGRAPH_PYWGCONDITION_H
