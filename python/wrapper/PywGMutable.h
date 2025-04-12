/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGMutable.h
@Time: 2025/4/8 23:19
@Desc: 
***************************/

#ifndef CGRAPH_PYWGMUTABLE_H
#define CGRAPH_PYWGMUTABLE_H

#include <pybind11/pybind11.h>

#include "CGraph.h"
#include "PyWrapperMacro.h"

class PywGMutable : public CGraph::GMutable {
public:
    explicit PywGMutable(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) {
        __addGElements_4py(elements);
    }
    ~PywGMutable() override = default;

    CStatus reshape(CGraph::GElementPtrArr& elements) override {
        PYBIND11_OVERLOAD_PURE(CStatus, PywGMutable, reshape, elements);
    }
};

PYCGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGMutable,
    CStatus reshape(CGraph::GElementPtrArr& elements) override {
        PYBIND11_OVERLOAD_PURE(CStatus, PywGMutableInterface, reshape, elements);
    });

#endif //CGRAPH_PYWGMUTABLE_H
