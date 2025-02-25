/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PyGCluster.h
@Time: 2025/1/30 21:52
@Desc: 
***************************/

#ifndef CGRAPH_PYGCLUSTER_H
#define CGRAPH_PYGCLUSTER_H

#include "CGraph.h"

class PyGCluster : public CGraph::GCluster {
public:
    explicit PyGCluster(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) {
        __addElements_4py(elements);
    }
    ~PyGCluster() override {};

    CStatus addGElements(const CGraph::GElementPtrArr& elements) {
        return __addElements_4py(elements);
    }
};

#endif //CGRAPH_PYGCLUSTER_H
