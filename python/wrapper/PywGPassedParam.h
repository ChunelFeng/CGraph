/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGPassedParam.h
@Time: 2025/3/22 13:22
@Desc: 
***************************/

#ifndef CGRAPH_PYWGPASSEDPARAM_H
#define CGRAPH_PYWGPASSEDPARAM_H

#include <pybind11/pybind11.h>

#include "CGraph.h"

class PywGPassedParam : public CGraph::GPassedParam {
public:
    explicit PywGPassedParam() = default;
    ~PywGPassedParam() override = default;

protected:
    CVoid clone(GPassedParam* param) override {
        PYBIND11_OVERLOAD_PURE(CVoid, GPassedParam, clone, param);
    }
};

#endif //CGRAPH_PYWGPASSEDPARAM_H
