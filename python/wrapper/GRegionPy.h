#ifndef CGRAPH_GREGION_PY_H
#define CGRAPH_GREGION_PY_H

#include "CGraph.h"

class GRegionPy : public CGraph::GRegion {
public:
    explicit GRegionPy() : CGraph::GRegion() {};
    ~GRegionPy() override {};

    CStatus addGElement(CGraph::GElementPtr element) {
        return addElement(element);
    }
};

#endif // CGRAPH_GREGION_PY_H
