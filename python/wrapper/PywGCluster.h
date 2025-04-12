/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGCluster.h
@Time: 2025/4/6 19:00
@Desc: 
***************************/

#ifndef CGRAPH_PYWGCLUSTER_H
#define CGRAPH_PYWGCLUSTER_H

#include "CGraph.h"
#include "PyWrapperMacro.h"

class PywGCluster : public CGraph::GCluster {
public:
    explicit PywGCluster(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) {
        __addGElements_4py(elements);
    }

    ~PywGCluster() override = default;
};

PYCGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGCluster, ;);

#endif //CGRAPH_PYWGCLUSTER_H
