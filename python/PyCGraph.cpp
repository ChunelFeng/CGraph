/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PyCGraph.cpp
@Time: 2025/1/30 21:43
@Desc:
***************************/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "CGraph.h"
#include "wrapper/PyWrapperInclude.h"

using namespace CGraph;
namespace py = pybind11;

PYBIND11_MODULE(PyCGraph, m) {
    py::class_<CStatus>(m, "CStatus")
        .def(py::init<>())
        .def(py::init<int, const std::string&>())
        .def("getCode", &CStatus::getCode)
        .def("getInfo", &CStatus::getInfo)
        .def("isOK", &CStatus::isOK);

    py::enum_<GMultiConditionType>(m, "GMultiConditionType")
        .value("SERIAL", GMultiConditionType::SERIAL)
        .value("PARALLEL", GMultiConditionType::PARALLEL)
        .export_values();

    py::class_<GParam, PywGParam, std::unique_ptr<GParam, py::nodelete> >(m, "GParam")
        .def(py::init<>());

    py::class_<PyGPipeline, std::unique_ptr<PyGPipeline, py::nodelete> >(m, "GPipeline")
        .def(py::init<>())
        .def("init", &PyGPipeline::init)
        .def("run", &PyGPipeline::run, py::call_guard<py::gil_scoped_release>())
        .def("process", &PyGPipeline::process, py::call_guard<py::gil_scoped_release>(),
             py::arg("runTimes") = 1)
        .def("destroy", &PyGPipeline::destroy)
        .def("registerGElement", &PyGPipeline::registerGElement,
            py::arg("element"),
            py::arg("depends") = GElementPtrSet{},
            py::arg("name") = CGRAPH_EMPTY,
            py::arg("loop") = CGRAPH_DEFAULT_LOOP_TIMES,
            "register a GElement with dependencies, name, and loop count.");

    py::class_<GElement, PywGElement, std::unique_ptr<GElement, py::nodelete> >(m, "GElement")
        .def(py::init<>())
        .def("createGParam", &GElement::__createGParam_4py)
        .def("getGParam", &GElement::__getGParam_4py)
        .def("getName", &GElement::getName)
        .def("setName", &GElement::setName)
        .def("addDependGElements", &GElement::addDependGElements,
            py::arg("elements"))
        .def("setLoop", &GElement::setLoop);

    py::class_<GNode, PywGNode, GElement, std::unique_ptr<GNode, py::nodelete> >(m, "GNode")
        .def(py::init<const std::string&, int>(),
             py::arg("name"),
             py::arg("loop") = CGRAPH_DEFAULT_LOOP_TIMES)
        .def(py::init<const GElementPtrSet&, const std::string&, int>(),
             py::arg("depends") = GElementPtrSet{},
             py::arg("name") = CGRAPH_EMPTY,
             py::arg("loop") = CGRAPH_DEFAULT_LOOP_TIMES);

    py::class_<PyGCluster, GElement, std::unique_ptr<PyGCluster, py::nodelete> >(m, "GCluster")
        .def(py::init<const CGraph::GElementPtrArr&>(),
            py::arg("elements") = GElementPtrArr{})
        .def("addGElements", &PyGCluster::addGElements,
            py::arg("elements"));

    py::class_<PyGRegion, GElement, std::unique_ptr<PyGRegion, py::nodelete> >(m, "GRegion")
        .def(py::init<const CGraph::GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{})
        .def("addGElements", &PyGRegion::addGElements,
            py::arg("elements"));

    py::class_<PywGCondition, GElement, std::unique_ptr<PywGCondition, py::nodelete> >(m, "GCondition")
        .def(py::init<const CGraph::GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{})
        .def("getRange", &PywGCondition::__getRange_4py)
        .def("addGElements", &PywGCondition::addGElements,
            py::arg("elements"));

    py::class_<PyGMultiCondition<CGraph::GMultiConditionType::SERIAL>, GElement>(m, "GSerialMultiCondition")
        .def(py::init<>())
        .def("addGElements", &PyGMultiCondition<CGraph::GMultiConditionType::SERIAL>::addGElements,
            py::arg("elements"));

    py::class_<PyGMultiCondition<CGraph::GMultiConditionType::PARALLEL>, GElement>(m, "GParallelMultiCondition")
        .def(py::init<>())
        .def("addGElements", &PyGMultiCondition<CGraph::GMultiConditionType::PARALLEL>::addGElements,
            py::arg("elements"));
}
