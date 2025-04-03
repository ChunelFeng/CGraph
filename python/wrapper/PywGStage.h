/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGStage.h
@Time: 2025/4/4 00:47
@Desc: 
***************************/

#ifndef CGRAPH_PYWGSTAGE_H
#define CGRAPH_PYWGSTAGE_H

#include <pybind11/pybind11.h>

#include "CGraph.h"

class PywGStage : public CGraph::GStage {
public:
    explicit PywGStage() = default;
    ~PywGStage() override = default;

protected:
    CVoid launch(CGraph::GStageParamPtr param) override {
        PYBIND11_OVERLOAD(CVoid, GStage, launch, param);
    }
};

#endif //CGRAPH_PYWGSTAGE_H
