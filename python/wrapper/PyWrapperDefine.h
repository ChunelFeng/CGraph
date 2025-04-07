/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PyWrapperDefine.h
@Time: 2025/4/6 14:48
@Desc: 
***************************/

#ifndef CGRAPH_PYWRAPPERDEFINE_H
#define CGRAPH_PYWRAPPERDEFINE_H

#include <pybind11/pybind11.h>

#include "CGraph.h"

#define PYCGRAPH_DECLARE_GELEMENT_COMMON_FUNCTIONS(PCGE)    \
    CBool isHold() override {                               \
        PYBIND11_OVERLOAD(CBool, PCGE, isHold);             \
    }                                                       \
                                                            \
    CBool isMatch() override {                              \
        PYBIND11_OVERLOAD(CBool, PCGE, isMatch);            \
    }                                                       \
                                                            \
    CStatus prepareRun() override {                         \
        PYBIND11_OVERLOAD(CStatus, PCGE, prepareRun);       \
    }                                                       \
                                                            \
    CStatus checkRunResult() override {                     \
        PYBIND11_OVERLOAD(CStatus, PCGE, checkRunResult);   \
    }                                                       \
                                                            \


#define PYCGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PCGG, PVFunc)                             \
class PCGG##Interface : public PCGG {                                                     \
public:                                                                                   \
    explicit PCGG##Interface(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) : PCGG(elements) {}; \
    ~PCGG##Interface() override = default;                                                \
protected:                                                                                \
    PYCGRAPH_DECLARE_GELEMENT_COMMON_FUNCTIONS(PCGG##Interface)                           \
    PVFunc                                                                                \
};                                                                                        \
                                                                                          \


#define PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(PCGG)                                  \
    py::class_<Pyw##PCGG, GElement, std::unique_ptr<Pyw##PCGG, py::nodelete> >(m, #PCGG)  \
        .def(py::init<const GElementPtrArr&>(),                                           \
            py::arg("elements") = GElementPtrArr{},                                       \
            py::keep_alive<1, 2>())                                                       \
        .def("addGElements", &Pyw##PCGG::__addGElements_4py,                              \
            py::arg("elements"),                                                          \
            py::keep_alive<1, 2>())                                                       \
                                                                                          \


#define PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(PCGO)                                      \
    def("createGParam", &PCGO::__createGParam_4py,                                        \
         py::keep_alive<1, 2>(),                                                          \
         py::call_guard<py::gil_scoped_release>())                                        \
    .def("getGParam", &PCGO::__getGParam_4py)                                             \
    .def("getGParamWithNoEmpty", &PCGO::__getGParamWithNoEmpty_4py)                       \
    .def("removeGParam", &PCGO::__removeGParam_4py,                                       \
         py::call_guard<py::gil_scoped_release>())                                        \
    .def("hasGParam", &PCGO::__hasGParam_4py,                                             \
         py::call_guard<py::gil_scoped_release>())                                        \
                                                                                          \

#endif //CGRAPH_PYWRAPPERDEFINE_H
