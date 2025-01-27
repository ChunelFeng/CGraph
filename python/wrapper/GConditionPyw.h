#ifndef CGRAPH_GCONDITION_PYW_H
#define CGRAPH_GCONDITION_PYW_H

#include <pybind11/pybind11.h>

#include "CGraph.h"

namespace py = pybind11;

class GConditionPyw : public CGraph::GCondition {
public:
    explicit GConditionPyw() : CGraph::GCondition() {};
    ~GConditionPyw() override {};

    CIndex choose() override {
        PYBIND11_OVERLOAD_PURE(CIndex, GConditionPyw, choose);
    }

    CStatus addGElement(CGraph::GElementPtr element) {
        return addElement(element);
    }
};

#endif // CGRAPH_GCONDITION_PYW_H