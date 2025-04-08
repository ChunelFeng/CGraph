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

    py::class_<GElementRelation>(m, "GElementRelation")
        .def(py::init<>())
        .def_readonly("predecessors", &GElementRelation::predecessors_)
        .def_readonly("successors", &GElementRelation::successors_)
        .def_readonly("children", &GElementRelation::children_)
        .def_readonly("belong", &GElementRelation::belong_);

    py::class_<UThreadPool>(m, "UThreadPool")
        .def(py::init<CBool, const UThreadPoolConfig&>(),
             py::arg("autoInit") = true,
             py::arg("config") = UThreadPoolConfig{})
         .def("setConfig", &UThreadPool::setConfig,
              py::keep_alive<1, 2>())
         .def("getConfig", &UThreadPool::getConfig)
         .def("init", &UThreadPool::init)
         .def("destroy", &UThreadPool::destroy)
         .def("isInit", &UThreadPool::isInit);

    py::enum_<GEngineType>(m, "GEngineType")
        .value("DYNAMIC", GEngineType::DYNAMIC)
        .value("TOPO", GEngineType::TOPO)
        .value("STATIC", GEngineType::STATIC)
        .export_values();

    py::enum_<GElementTimeoutStrategy>(m, "GElementTimeoutStrategy")
        .value("AS_ERROR", GElementTimeoutStrategy::AS_ERROR)
        .value("HOLD_BY_PIPELINE", GElementTimeoutStrategy::HOLD_BY_PIPELINE)
        .value("NO_HOLD", GElementTimeoutStrategy::NO_HOLD);

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
        .value("ASYNC", std::launch::async)
        .value("DEFERRED", std::launch::deferred)
        .export_values();

    py::class_<std::shared_future<void> >(m, "StdSharedFutureVoid")
        .def("wait", [] (std::shared_future<void>& fut) {
            fut.wait();
        }, py::call_guard<py::gil_scoped_release>());

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
        .def("__iadd__", &CStatus::operator+=,
             py::return_value_policy::reference)
        .def("getCode", &CStatus::getCode)
        .def("getInfo", &CStatus::getInfo)
        .def("isOK", &CStatus::isOK)
        .def("isErr", &CStatus::isErr);

    py::class_<GAspect, PywGAspect, std::unique_ptr<GAspect, py::nodelete> >(m, "GAspect")
        .def(py::init<>())
        .def("getName", &GAspect::__getName_4py)
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GAspect);

    py::class_<GDaemon, PywGDaemon, std::unique_ptr<GDaemon, py::nodelete> >(m, "GDaemon")
        .def(py::init<>())
        .def("getInterval", &GDaemon::__getInterval_4py)
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GDaemon);

    py::class_<GEvent, PywGEvent, std::unique_ptr<GEvent, py::nodelete> >(m, "GEvent")
        .def(py::init<>())
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GEvent);

    py::class_<GStage, PywGStage, std::unique_ptr<GStage, py::nodelete> >(m, "GStage")
        .def(py::init<>())
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GStage);

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
    m.attr("GDaemonParam") = m.attr("GPassedParam");
    m.attr("GStageParam") = m.attr("GPassedParam");
    m.attr("GEventParam") = m.attr("GPassedParam");

    py::class_<PyGPipeline>(m, "GPipeline")
        .def(py::init<>())
        .def("init", &PyGPipeline::init)
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(PyGPipeline)
        .def("setUniqueThreadPoolConfig", &PyGPipeline::setUniqueThreadPoolConfig)
        .def("setSharedThreadPool", &PyGPipeline::setSharedThreadPool,
             py::call_guard<py::gil_scoped_release>(),
             py::keep_alive<1, 2>())
        .def("setGEngineType", &PyGPipeline::setGEngineType)
        .def("run", &PyGPipeline::run,
             py::call_guard<py::gil_scoped_release>())
        .def("process", &PyGPipeline::process,
             py::call_guard<py::gil_scoped_release>(),
             py::arg("runTimes") = 1)
        .def("destroy", &PyGPipeline::destroy)
        .def("addGEvent", &PyGPipeline::__addGEvent_4py,
             py::keep_alive<1, 2>())
        .def("addGDaemon", &PyGPipeline::__addGDaemon_4py,
             py::arg("daemon"),
             py::arg("ms"),
             py::keep_alive<1, 2>())
        .def("addGStage", &PyGPipeline::__addGStage_4py,
             py::arg("stage"),
             py::arg("key"),
             py::arg("threshold"),
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
        .def("perf", &PyGPipeline::__perf_4py,
             py::call_guard<py::gil_scoped_release>())
        .def("dump", &PyGPipeline::__dump_4py)
        .def("trim", &PyGPipeline::trim)
        .def("registerGElement", &PyGPipeline::__registerGElement_4py,
             py::arg("element"),
             py::arg("depends") = GElementPtrSet{},
             py::arg("name") = CGRAPH_EMPTY,
             py::arg("loop") = CGRAPH_DEFAULT_LOOP_TIMES,
             py::keep_alive<1, 2>());

    py::class_<GElement, PywGElement, std::unique_ptr<GElement, py::nodelete> >(m, "GElement")
        .def(py::init<>())
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GElement)
        .def("notify", &GElement::__notify_4py,
             py::arg("key"),
             py::arg("type"),
             py::arg("strategy") = GEventAsyncStrategy::PIPELINE_RUN_FINISH,
             py::call_guard<py::gil_scoped_release>())
        .def("asyncNotify", &GElement::__asyncNotify_4py,
             py::arg("key"),
             py::arg("strategy") = GEventAsyncStrategy::PIPELINE_RUN_FINISH,
             py::call_guard<py::gil_scoped_release>())
         .def("enterStage", &GElement::__enterStage_4py,
             py::arg("key"),
             py::call_guard<py::gil_scoped_release>())
        .def("getName", &GElement::getName)
        .def("setLoop", &GElement::setLoop,
             py::arg("loop"))
        .def("setName", &GElement::setName,
             py::arg("name"))
        .def("setLevel", &GElement::setLevel,
             py::arg("level"))
        .def("setVisible", &GElement::setVisible,
             py::arg("visible"))
        .def("setMacro", &GElement::setMacro,
            py::arg("macro"))
        .def("setTimeout", &GElement::setTimeout,
             py::arg("timeout"),
             py::arg("strategy") = GElementTimeoutStrategy::AS_ERROR)
        .def("isTimeout", &GElement::__isTimeout_4py)
        .def("isGGroup", &GElement::isGGroup)
        .def("isGAdaptor", &GElement::isGAdaptor)
        .def("isGNode", &GElement::isGNode)
        .def("getRelation", &GElement::getRelation)
        .def("getCurState", &GElement::getCurState)
        .def("addGAspect", &GElement::__addGAspect_4py,
             py::keep_alive<1, 2>())
        .def("addDependGElements", &GElement::addDependGElements,
             py::arg("elements"))
        .def("removeDepend", &GElement::removeDepend,
             py::arg("element"));

    py::class_<GNode, PywGNode, GElement, std::unique_ptr<GNode, py::nodelete> >(m, "GNode")
        .def(py::init<const std::string&, int>(),
             py::arg("name"),
             py::arg("loop") = CGRAPH_DEFAULT_LOOP_TIMES)
        .def(py::init<const GElementPtrSet&, const std::string&, int>(),
             py::arg("depends") = GElementPtrSet{},
             py::arg("name") = CGRAPH_EMPTY,
             py::arg("loop") = CGRAPH_DEFAULT_LOOP_TIMES);

    py::class_<PywGCluster, GElement, std::unique_ptr<PywGCluster, py::nodelete> >(m, "GCluster")
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PywGCluster::__addGElements_4py,
             py::arg("elements"),
             py::keep_alive<1, 2>());
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GClusterInterface);

    py::class_<PywGRegion, GElement, std::unique_ptr<PywGRegion, py::nodelete> >(m, "GRegion")
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PywGRegion::__addGElements_4py,
             py::arg("elements"),
             py::keep_alive<1, 2>());
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GRegionInterface);

    py::class_<PywGCondition, GElement, std::unique_ptr<PywGCondition, py::nodelete> >(m, "GCondition")
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("getRange", &PywGCondition::__getRange_4py)
        .def("addGElements", &PywGCondition::__addGElements_4py,
            py::arg("elements"),
            py::keep_alive<1, 2>());
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GConditionInterface);

    py::class_<PywGSerialMultiCondition, GElement, std::unique_ptr<PywGSerialMultiCondition, py::nodelete> >(
            m, "GSerialMultiCondition")
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PywGSerialMultiCondition::__addGElements_4py,
             py::arg("elements"),
             py::keep_alive<1, 2>());
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GSerialMultiConditionInterface);

    py::class_<PywGParallelMultiCondition, GElement, std::unique_ptr<PywGParallelMultiCondition, py::nodelete> >(
            m, "GParallelMultiCondition")
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PywGParallelMultiCondition::__addGElements_4py,
             py::arg("elements"),
             py::keep_alive<1, 2>());
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GParallelMultiConditionInterface);

    py::class_<PywGMutable, GElement, std::unique_ptr<PywGMutable, py::nodelete> >(m, "GMutable")
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PywGMutable::__addGElements_4py,
             py::arg("elements"),
             py::keep_alive<1, 2>());
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GMutableInterface);
}
