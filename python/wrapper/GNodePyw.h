#ifndef CGRAPH_GNODE_PYW_H
#define CGRAPH_GNODE_PYW_H

#include <pybind11/pybind11.h>

#include "CGraph.h"

namespace py = pybind11;

class GNodePyw : public CGraph::GNode {
protected:
    CStatus init() override {
        PYBIND11_OVERLOAD(CStatus, GNode, init);
    }

    CStatus run() override {
        PYBIND11_OVERLOAD_PURE(CStatus, GNode, run);
    }

    CStatus destroy() override {
        PYBIND11_OVERLOAD(CStatus, GNode, destroy);
    }

    CBool isHold() override {
        PYBIND11_OVERLOAD(CBool, GNode, isHold);
    }

    CBool isMatch() override {
        PYBIND11_OVERLOAD(CBool, GNode, isMatch);
    }
};

#endif // CGRAPH_GNODE_PYW_H
