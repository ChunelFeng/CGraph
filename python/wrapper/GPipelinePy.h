#ifndef CGRAPH_GPIPELINE_PY_H
#define CGRAPH_GPIPELINE_PY_H

#include "CGraph.h"

class GPipelinePy : public CGraph::GPipeline {
public:
    explicit GPipelinePy() : CGraph::GPipeline() {}
    ~GPipelinePy() override {}

    CStatus registerGElement(CGraph::GElementPtr element,
                             const CGraph::GElementPtrSet &depends = CGraph::GElementPtrSet{},
                             const std::string &name = CGraph::CGRAPH_EMPTY,
                             CSize loop = CGraph::CGRAPH_DEFAULT_LOOP_TIMES) {
        return innerRegister(element, depends, name, loop);
    }
};

#endif // CGRAPH_GPIPELINE_PY_H
