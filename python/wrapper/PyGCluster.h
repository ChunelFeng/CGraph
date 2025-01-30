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
    explicit PyGCluster() : CGraph::GCluster() {};
    ~PyGCluster() override {};

    CStatus addGElement(CGraph::GElementPtr element) {
        return addElement(element);
    }
};

#endif //CGRAPH_PYGCLUSTER_H
