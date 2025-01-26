#ifndef CGRAPH_GPIPELINEPY_H
#define CGRAPH_GPIPELINEPY_H

#include "CGraph.h"

class GPipelinePy : public CGraph::GPipeline {
public:
    explicit GPipelinePy() : CGraph::GPipeline() {}
    virtual ~GPipelinePy() {}

    CStatus registerGElement(CGraph::GElementPtr element,
                             const CGraph::GElementPtrSet &depends = CGraph::GElementPtrSet{},
                             const std::string &name = CGraph::CGRAPH_EMPTY,
                             CSize loop = CGraph::CGRAPH_DEFAULT_LOOP_TIMES) {
        return innerRegister(element, depends, name, loop);
    }
};

#endif // CGRAPH_GPIPELINEPY_H
