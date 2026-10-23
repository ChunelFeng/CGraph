/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGElement.h
@Time: 2025/1/30 21:39
@Desc: 
***************************/

#ifndef CGRAPH_PYWGELEMENT_H
#define CGRAPH_PYWGELEMENT_H

#include <pybind11/pybind11.h>

#include "CGraph.h"
#include "PyWrapperMacro.h"

class PywGElement : public CGraph::GElement {
protected:
    CStatus run() override {
        PYCGRAPH_OVERRIDE_PURE_RET_CSTATUS(GElement, run, );
    }
};

#endif //CGRAPH_PYWGELEMENT_H
