/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PyGFence.h
@Time: 2025/4/10 00:27
@Desc: 
***************************/

#ifndef CGRAPH_PYGFENCE_H
#define CGRAPH_PYGFENCE_H

#include "CGraph.h"

class PyGFence : public CGraph::GFence {
public:
    explicit PyGFence() = default;

    ~PyGFence() override = default;
};

#endif //CGRAPH_PYGFENCE_H
