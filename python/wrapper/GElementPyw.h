#ifndef CGRAPH_GELEMENTPY_H
#define CGRAPH_GELEMENTPY_H

#include <pybind11/pybind11.h>

#include "CGraph.h"

namespace py = pybind11;

class GElementPyw : public CGraph::GElement {
protected:
    CStatus run() override {
        PYBIND11_OVERLOAD_PURE(CStatus, GElement, run);
    }
};

#endif // CGRAPH_GELEMENTPY_H
