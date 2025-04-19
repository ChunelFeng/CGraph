/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGPipelineDeleter.h
@Time: 2025/4/19 12:37
@Desc: 
***************************/

#ifndef CGRAPH_PYWGPIPELINEDELETER_H
#define CGRAPH_PYWGPIPELINEDELETER_H

#include "CGraph.h"

struct PywGPipelineDeleter {
    void operator()(CGraph::GPipelinePtr ptr) {
        CGraph::GPipelineFactory::remove(ptr);
    }
};

#endif //CGRAPH_PYWGPIPELINEDELETER_H
