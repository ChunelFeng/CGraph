#ifndef CGRAPH_GNODEPY_H
#define CGRAPH_GNODEPY_H

#include <pybind11/pybind11.h>

#include "CGraph.h"

namespace py = pybind11;

class GNodePyw : public CGraph::GNode {
protected:
    CStatus run() override {
        PYBIND11_OVERLOAD_PURE(CStatus, GNode, run);
    }

    CStatus init() override {
        PYBIND11_OVERLOAD(CStatus, GNode, init);
    }

    CStatus destroy() override {
        PYBIND11_OVERLOAD(CStatus, GNode, destroy);
    }

    CBool isHold() override {
        PYBIND11_OVERLOAD(CBool, GNode, isHold);
    }
};

#endif // CGRAPH_GNODEPY_H
