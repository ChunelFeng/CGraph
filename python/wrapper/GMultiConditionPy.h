#ifndef CGRAPH_GMULTICONDITION_PY_H
#define CGRAPH_GMULTICONDITION_PY_H

#include "CGraph.h"

template<CGraph::GMultiConditionType type>
class GMultiConditionPy : public CGraph::GMultiCondition<type> {
public:
    explicit GMultiConditionPy() : CGraph::GMultiCondition<type>() {};
    ~GMultiConditionPy() override {};

    CStatus addGElement(CGraph::GElementPtr element) {
        return CGraph::GMultiCondition<type>::addElement(element);
    }
};

#endif // CGRAPH_GMULTICONDITION_PY_H