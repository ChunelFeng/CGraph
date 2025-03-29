/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGEvent.h
@Time: 2025/3/19 23:35
@Desc: 
***************************/

#ifndef CGRAPH_PYWGEVENT_H
#define CGRAPH_PYWGEVENT_H

#include <pybind11/pybind11.h>

#include "CGraph.h"

class PywGEvent : public CGraph::GEvent {
public:
    explicit PywGEvent() = default;

protected:
    CVoid trigger(CGraph::GEventParamPtr param) override {
        PYBIND11_OVERLOAD_PURE(CVoid, GEvent, trigger, param);
    }

    CStatus init() override {
        PYBIND11_OVERLOAD(CStatus, GEvent, init);
    }

    CStatus destroy() override {
        PYBIND11_OVERLOAD(CStatus, GEvent, destroy);
    }
};

#endif //CGRAPH_PYWGEVENT_H
