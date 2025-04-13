/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGFunction.h
@Time: 2025/4/13 18:38
@Desc: 
***************************/

#ifndef CGRAPH_PYWGFUNCTION_H
#define CGRAPH_PYWGFUNCTION_H


#include "CGraph.h"

class PywGFunction : public CGraph::GFunction {
public:
    explicit PywGFunction() = default;

    ~PywGFunction() override = default;
};

#endif //CGRAPH_PYWGFUNCTION_H
