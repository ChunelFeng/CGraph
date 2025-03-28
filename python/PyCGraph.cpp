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

    py::enum_<GEventType>(m, "GEventType")
        .value("SYNC", GEventType::SYNC)
        .value("ASYNC", GEventType::ASYNC)
        .export_values();

    py::enum_<GEventAsyncStrategy>(m, "GEventAsyncStrategy")
        .value("PIPELINE_RUN_FINISH", GEventAsyncStrategy::PIPELINE_RUN_FINISH)
        .value("PIPELINE_DESTROY", GEventAsyncStrategy::PIPELINE_DESTROY)
        .value("NO_WAIT", GEventAsyncStrategy::NO_WAIT)
        .export_values();

    py::enum_<std::launch>(m, "StdLaunchPolicy")
        .value("async", std::launch::async)
        .value("deferred", std::launch::deferred)
        .export_values();

    py::class_<std::shared_future<void> >(m, "StdSharedFutureVoid")
        .def("wait", [] (std::shared_future<void>& sfVoid) {
            sfVoid.wait();
        },
        py::call_guard<py::gil_scoped_release>());

    py::class_<std::future<CStatus> >(m, "StdFutureCStatus")
        .def("get", [] (std::future<CStatus>& fut) {
            return fut.get();
        }, py::call_guard<py::gil_scoped_release>())
        .def("wait", [] (std::future<CStatus>& fut) {
            fut.wait();
        }, py::call_guard<py::gil_scoped_release>());

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
        .def("createGParam", &GAspect::__createGParam_4py,
             py::call_guard<py::gil_scoped_release>(),
             py::keep_alive<1, 2>())
        .def("getGParam", &GAspect::__getGParam_4py)
        .def("getGParamWithNoEmpty", &GAspect::__getGParamWithNoEmpty_4py)
        .def("removeGParam", &GAspect::__removeGParam_4py,
             py::call_guard<py::gil_scoped_release>());

    py::class_<GEvent, PywGEvent, std::unique_ptr<GEvent, py::nodelete> >(m, "GEvent")
        .def(py::init<>())
        .def("createGParam", &GEvent::__createGParam_4py,
             py::call_guard<py::gil_scoped_release>(),
             py::keep_alive<1, 2>())
        .def("getGParam", &GEvent::__getGParam_4py)
        .def("getGParamWithNoEmpty", &GEvent::__getGParamWithNoEmpty_4py)
        .def("removeGParam", &GEvent::__removeGParam_4py,
             py::call_guard<py::gil_scoped_release>());

    py::class_<GParam, PywGParam, std::unique_ptr<GParam, py::nodelete> >(m, "GParam")
        .def(py::init<>())
        .def("lock", &GParam::lock,
             py::call_guard<py::gil_scoped_release>())
        .def("unlock", &GParam::unlock,
             py::call_guard<py::gil_scoped_release>())
        .def("tryLock", &GParam::tryLock,
             py::call_guard<py::gil_scoped_release>());

    py::class_<GPassedParam, PywGPassedParam, std::unique_ptr<GPassedParam, py::nodelete> >(m, "GPassedParam")
        .def(py::init<>());
    m.attr("GElementParam") = m.attr("GPassedParam");

    py::class_<PyGPipeline>(m, "GPipeline")
        .def(py::init<>())
        .def("init", &PyGPipeline::init)
        .def("createGParam", &PyGPipeline::__createGParam_4py,
             py::call_guard<py::gil_scoped_release>(),
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
        .def("addGEvent", &PyGPipeline::__addGEvent_4py,
             py::keep_alive<1, 2>())
        .def("asyncRun", &PyGPipeline::asyncRun,
             py::arg("policy") = std::launch::async,
             py::call_guard<py::gil_scoped_release>())
        .def("asyncProcess", &PyGPipeline::asyncProcess,
             py::arg("runTimes") = CGRAPH_DEFAULT_LOOP_TIMES,
             py::arg("policy") = std::launch::async,
             py::call_guard<py::gil_scoped_release>())
        .def("cancel", &PyGPipeline::cancel,
            py::call_guard<py::gil_scoped_release>())
        .def("yields", &PyGPipeline::yield,    // yield is python's keyword, use yields to replace
             py::call_guard<py::gil_scoped_release>())
        .def("resume", &PyGPipeline::resume,
             py::call_guard<py::gil_scoped_release>())
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
        .def("notify", &GElement::__notify_4py,
             py::arg("key"),
             py::arg("type"),
             py::arg("strategy") = GEventAsyncStrategy::PIPELINE_RUN_FINISH,
             py::call_guard<py::gil_scoped_release>())
        .def("asyncNotify", &GElement::__asyncNotify_4py,
             py::arg("key"),
             py::arg("strategy") = GEventAsyncStrategy::PIPELINE_RUN_FINISH,
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
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PyGCluster::__addGElements_4py,
             py::arg("elements"),
             py::keep_alive<1, 2>());

    py::class_<PyGRegion, GElement, std::unique_ptr<PyGRegion, py::nodelete> >(m, "GRegion")
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PyGRegion::__addGElements_4py,
             py::arg("elements"),
             py::keep_alive<1, 2>());

    py::class_<PywGCondition, GElement, std::unique_ptr<PywGCondition, py::nodelete> >(m, "GCondition")
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("getRange", &PywGCondition::__getRange_4py)
        .def("addGElements", &PywGCondition::__addGElements_4py,
            py::arg("elements"),
            py::keep_alive<1, 2>());

    py::class_<PyGMultiCondition<GMultiConditionType::SERIAL>,
            GElement, std::unique_ptr<PyGMultiCondition<GMultiConditionType::SERIAL>,
            py::nodelete> >(m, "GSerialMultiCondition")
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PyGMultiCondition<GMultiConditionType::SERIAL>::__addGElements_4py,
             py::arg("elements"),
             py::keep_alive<1, 2>());

    py::class_<PyGMultiCondition<GMultiConditionType::PARALLEL>,
            GElement, std::unique_ptr<PyGMultiCondition<GMultiConditionType::PARALLEL>,
            py::nodelete> >(m, "GParallelMultiCondition")
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PyGMultiCondition<GMultiConditionType::PARALLEL>::__addGElements_4py,
            py::arg("elements"),
            py::keep_alive<1, 2>());
}
