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
    explicit PyGPipeline() : CGraph::GPipeline() {}
    ~PyGPipeline() override {}

    CStatus registerGElement(CGraph::GElementPtr element,
                             const CGraph::GElementPtrSet &depends = CGraph::GElementPtrSet{},
                             const std::string &name = CGraph::CGRAPH_EMPTY,
                             CSize loop = CGraph::CGRAPH_DEFAULT_LOOP_TIMES) {
        return innerRegister(element, depends, name, loop);
    }
};

#endif //CGRAPH_PYGPIPELINE_H
