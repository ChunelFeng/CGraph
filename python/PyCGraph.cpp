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
    py::class_<UThreadPoolConfig>(m, "UThreadPoolConfig")
        .def(pybind11::init<>())
        .def_readwrite("default_thread_size", &UThreadPoolConfig::default_thread_size_)
        .def_readwrite("secondary_thread_size", &UThreadPoolConfig::secondary_thread_size_)
        .def_readwrite("max_thread_size", &UThreadPoolConfig::max_thread_size_)
        .def_readwrite("max_task_steal_range", &UThreadPoolConfig::max_task_steal_range_)
        .def_readwrite("max_local_batch_size", &UThreadPoolConfig::max_local_batch_size_)
        .def_readwrite("max_pool_batch_size", &UThreadPoolConfig::max_pool_batch_size_)
        .def_readwrite("max_steal_batch_size", &UThreadPoolConfig::max_steal_batch_size_)
        .def_readwrite("primary_thread_busy_epoch", &UThreadPoolConfig::primary_thread_busy_epoch_)
        .def_readwrite("primary_thread_empty_interval", &UThreadPoolConfig::primary_thread_empty_interval_)
        .def_readwrite("secondary_thread_ttl", &UThreadPoolConfig::secondary_thread_ttl_)
        .def_readwrite("monitor_span", &UThreadPoolConfig::monitor_span_)
        .def_readwrite("queue_empty_interval", &UThreadPoolConfig::queue_emtpy_interval_)
        .def_readwrite("primary_thread_policy", &UThreadPoolConfig::primary_thread_policy_)
        .def_readwrite("secondary_thread_policy", &UThreadPoolConfig::secondary_thread_policy_)
        .def_readwrite("primary_thread_priority", &UThreadPoolConfig::primary_thread_priority_)
        .def_readwrite("secondary_thread_priority", &UThreadPoolConfig::secondary_thread_priority_)
        .def_readwrite("bind_cpu_enable", &UThreadPoolConfig::bind_cpu_enable_)
        .def_readwrite("batch_task_enable", &UThreadPoolConfig::batch_task_enable_)
        .def_readwrite("monitor_enable", &UThreadPoolConfig::monitor_enable_);

    py::enum_<GEngineType>(m, "GEngineType")
        .value("DYNAMIC", GEngineType::DYNAMIC)
        .value("TOPO", GEngineType::TOPO)
        .value("STATIC", GEngineType::STATIC)
        .export_values();

    py::enum_<GMultiConditionType>(m, "GMultiConditionType")
        .value("SERIAL", GMultiConditionType::SERIAL)
        .value("PARALLEL", GMultiConditionType::PARALLEL)
        .export_values();

    py::class_<CStatus>(m, "CStatus")
        .def(py::init<>())
        .def(py::init<int, const std::string&>())
        .def("getCode", &CStatus::getCode)
        .def("getInfo", &CStatus::getInfo)
        .def("isOK", &CStatus::isOK)
        .def("isErr", &CStatus::isErr);

    py::class_<GAspect, PywGAspect, std::unique_ptr<GAspect, py::nodelete> >(m, "GAspect")
        .def(py::init<>())
        .def("getName", &GAspect::__getName_4py)
        .def("getGParam", &GAspect::__getGParam_4py)
        .def("getGParamWithNoEmpty", &GAspect::__getGParamWithNoEmpty_4py);

    py::class_<GParam, PywGParam, std::unique_ptr<GParam, py::nodelete> >(m, "GParam")
        .def(py::init<>())
        .def("lock", &GParam::lock,
             py::call_guard<py::gil_scoped_release>())
        .def("unlock", &GParam::unlock,
             py::call_guard<py::gil_scoped_release>())
        .def("tryLock", &GParam::tryLock,
             py::call_guard<py::gil_scoped_release>());

    py::class_<PyGPipeline, std::unique_ptr<PyGPipeline, py::nodelete> >(m, "GPipeline")
        .def(py::init<>())
        .def("init", &PyGPipeline::init)
        .def("createGParam", &PyGPipeline::__createGParam_4py,
             py::keep_alive<1, 2>())
        .def("getGParam", &PyGPipeline::__getGParam_4py)
        .def("getGParamWithNoEmpty", &PyGPipeline::__getGParamWithNoEmpty_4py)
        .def("removeGParam", &PyGPipeline::__removeGParam_4py,
             py::call_guard<py::gil_scoped_release>())
        .def("hasGParam", &PyGPipeline::__hasGParam_4py,
             py::call_guard<py::gil_scoped_release>())
        .def("setUniqueThreadPoolConfig", &PyGPipeline::setUniqueThreadPoolConfig)
        .def("setGEngineType", &PyGPipeline::setGEngineType)
        .def("run", &PyGPipeline::run,
             py::call_guard<py::gil_scoped_release>())
        .def("process", &PyGPipeline::process,
             py::call_guard<py::gil_scoped_release>(),
             py::arg("runTimes") = 1)
        .def("destroy", &PyGPipeline::destroy)
        .def("registerGElement", &PyGPipeline::__registerGElement_4py,
            py::arg("element"),
            py::arg("depends") = GElementPtrSet{},
            py::arg("name") = CGRAPH_EMPTY,
            py::arg("loop") = CGRAPH_DEFAULT_LOOP_TIMES,
            py::keep_alive<1, 2>());

    py::class_<GElement, PywGElement, std::unique_ptr<GElement, py::nodelete> >(m, "GElement")
        .def(py::init<>())
        .def("createGParam", &GElement::__createGParam_4py,
             py::keep_alive<1, 2>(),
                     py::call_guard<py::gil_scoped_release>())
        .def("getGParam", &GElement::__getGParam_4py)
        .def("getGParamWithNoEmpty", &GElement::__getGParamWithNoEmpty_4py)
        .def("removeGParam", &GElement::__removeGParam_4py,
             py::call_guard<py::gil_scoped_release>())
        .def("hasGParam", &GElement::__hasGParam_4py,
             py::call_guard<py::gil_scoped_release>())
        .def("getName", &GElement::getName)
        .def("setName", &GElement::setName)
        .def("setLevel", &GElement::setLevel)
        .def("addGAspect", &GElement::__addGAspect_4py,
             py::keep_alive<1, 2>())
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
            py::arg("elements") = GElementPtrArr{},
            py::keep_alive<1, 2>())
        .def("addGElements", &PyGCluster::__addElements_4py,
            py::arg("elements"),
            py::keep_alive<1, 2>());

    py::class_<PyGRegion, GElement, std::unique_ptr<PyGRegion, py::nodelete> >(m, "GRegion")
        .def(py::init<const CGraph::GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PyGRegion::__addElements_4py,
            py::arg("elements"),
            py::keep_alive<1, 2>());

    py::class_<PywGCondition, GElement, std::unique_ptr<PywGCondition, py::nodelete> >(m, "GCondition")
        .def(py::init<const CGraph::GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("getRange", &PywGCondition::__getRange_4py)
        .def("addGElements", &PywGCondition::__addElements_4py,
            py::arg("elements"),
            py::keep_alive<1, 2>());

    py::class_<PyGMultiCondition<CGraph::GMultiConditionType::SERIAL>, GElement>(m, "GSerialMultiCondition")
        .def(py::init<const CGraph::GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PyGMultiCondition<CGraph::GMultiConditionType::SERIAL>::__addElements_4py,
            py::arg("elements"),
            py::keep_alive<1, 2>());

    py::class_<PyGMultiCondition<CGraph::GMultiConditionType::PARALLEL>, GElement>(m, "GParallelMultiCondition")
        .def(py::init<const CGraph::GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PyGMultiCondition<CGraph::GMultiConditionType::PARALLEL>::__addElements_4py,
            py::arg("elements"),
            py::keep_alive<1, 2>());
}
