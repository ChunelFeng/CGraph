/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PyGPipeline.h
@Time: 2025/1/30 21:43
@Desc: 
***************************/

#ifndef CGRAPH_PYGPIPELINE_H
#define CGRAPH_PYGPIPELINE_H

#include "CGraph.h"

class PyGPipeline : public CGraph::GPipeline {
public:
    explicit PyGPipeline() = default;

    ~PyGPipeline() override = default;
};

#endif //CGRAPH_PYGPIPELINE_H
