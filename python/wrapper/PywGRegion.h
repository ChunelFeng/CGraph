/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGRegion.h
@Time: 2025/4/6 19:03
@Desc: 
***************************/

#ifndef CGRAPH_PYWGREGION_H
#define CGRAPH_PYWGREGION_H

#include "CGraph.h"
#include "PyWrapperMacro.h"

class PywGRegion : public CGraph::GRegion {
public:
    explicit PywGRegion(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) {
        __addGElements_4py(elements);
    };

    ~PywGRegion() override = default;
};

PYCGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGRegion, ;);

#endif //CGRAPH_PYWGREGION_H
