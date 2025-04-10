/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGFence.h
@Time: 2025/4/11 00:05
@Desc: 
***************************/

#ifndef CGRAPH_PYWGFENCE_H
#define CGRAPH_PYWGFENCE_H


#include "CGraph.h"

class PywGFence : public CGraph::GFence {
public:
    explicit PywGFence() = default;

    ~PywGFence() override = default;
};

#endif //CGRAPH_PYWGFENCE_H
