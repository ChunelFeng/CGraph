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
        auto status = this->dump(oss);
        CGRAPH_THROW_EXCEPTION_BY_STATUS(status);
        return oss.str();
    }

    std::string __perf() {
        std::ostringstream oss;
        auto status = this->perf(oss);
        CGRAPH_THROW_EXCEPTION_BY_STATUS(status);
        return oss.str();
    }
};

#endif //CGRAPH_PYWGPIPELINE_H
