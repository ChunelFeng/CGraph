/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGPipeline.h
@Time: 2025/1/30 21:43
@Desc: 
***************************/

#ifndef CGRAPH_PYWGPIPELINE_H
#define CGRAPH_PYWGPIPELINE_H

#include <sstream>

#include "CGraph.h"

class PywGPipeline : public CGraph::GPipeline {
public:
    explicit PywGPipeline() = default;

    ~PywGPipeline() override = default;

    std::string __dump() {
        std::ostringstream oss;
        this->dump(oss);
        return oss.str();
    }

    std::string __perf() {
        std::ostringstream oss;
        this->perf(oss);
        return oss.str();
    }
};

#endif //CGRAPH_PYWGPIPELINE_H
