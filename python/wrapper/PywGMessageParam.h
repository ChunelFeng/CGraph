/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGMessageParam.h
@Time: 2026/10/20 23:42
@Desc: 
***************************/

#ifndef CGRAPH_PYWGMESSAGEPARAM_H
#define CGRAPH_PYWGMESSAGEPARAM_H

#include <pybind11/pybind11.h>
#include <pybind11/trampoline_self_life_support.h>

#include "CGraph.h"


struct PywGMessageParam : public CGraph::GMessageParam,
                          public pybind11::trampoline_self_life_support {
};

#endif //CGRAPH_PYWGMESSAGEPARAM_H
