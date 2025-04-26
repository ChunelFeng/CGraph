/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PyCGraph.cpp
@Time: 2025/1/30 21:43
@Desc:
***************************/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "CGraph.h"
#include "wrapper/PyWrapperInclude.h"

using namespace CGraph;
namespace py = pybind11;

PYBIND11_MODULE(PyCGraph, m) {
    m.doc() = "CGraph with python api, github: https://github.com/ChunelFeng/CGraph";

    py::class_<UThreadPoolConfig>(m, "UThreadPoolConfig")
        .def(py::init<>())
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
              py::arg("config"),
              py::keep_alive<1, 2>())
        .def("getConfig", &UThreadPool::getConfig)
        .def("init", &UThreadPool::init)
        .def("destroy", &UThreadPool::destroy)
        .def("isInit", &UThreadPool::isInit);

    py::register_exception<CException>(m, "CException");

    py::enum_<GEngineType>(m, "GEngineType")
        .value("DYNAMIC", GEngineType::DYNAMIC)
        .value("TOPO", GEngineType::TOPO)
        .value("STATIC", GEngineType::STATIC)
        .export_values();

    py::enum_<GElementTimeoutStrategy>(m, "GElementTimeoutStrategy")
        .value("AS_ERROR", GElementTimeoutStrategy::AS_ERROR)
        .value("HOLD_BY_PIPELINE", GElementTimeoutStrategy::HOLD_BY_PIPELINE)
        .value("NO_HOLD", GElementTimeoutStrategy::NO_HOLD)
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

    py::enum_<CFunctionType>(m, "CFunctionType")
        .value("INIT", CFunctionType::INIT)
        .value("RUN", CFunctionType::RUN)
        .value("DESTROY", CFunctionType::DESTROY)
        .export_values();

    py::enum_<GElementState>(m, "GElementState")
        .value("NORMAL", GElementState::NORMAL)
        .value("CANCEL", GElementState::CANCEL)
        .value("SUSPEND", GElementState::SUSPEND)
        .value("TIMEOUT", GElementState::TIMEOUT)
        .export_values();
    m.attr("GPipelineState") = m.attr("GElementState");

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
        .def(py::init<int, const std::string&>(),
             py::arg("errorCode"),
             py::arg("errorInfo"))
        .def("__iadd__", &CStatus::operator+=,
             py::return_value_policy::reference)
        .def("getCode", &CStatus::getCode)
        .def("getInfo", &CStatus::getInfo)
        .def("reset", &CStatus::reset)
        .def("isOK", &CStatus::isOK)
        .def("isErr", &CStatus::isErr)
        .def("isCrash", &CStatus::isCrash);

    py::class_<GAspect, PywGAspect, std::unique_ptr<GAspect, py::nodelete> >(m, "GAspect")
        .def(py::init<>())
        .def("getName", &GAspect::__getName_4py)
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GAspect)
        .PYCGRAPH_DEF_GEVENT_PYBIND11_FUNCTIONS(GAspect);

    py::class_<GDaemon, PywGDaemon, std::unique_ptr<GDaemon, py::nodelete> >(m, "GDaemon")
        .def(py::init<>())
        .def("getInterval", &GDaemon::__getInterval_4py)
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GDaemon)
        .PYCGRAPH_DEF_GEVENT_PYBIND11_FUNCTIONS(GDaemon);

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

    py::class_<GPipeline, std::unique_ptr<GPipeline, PywGPipelineDeleter> >(m, "GPipeline")
        .def(py::init<>([]() { return GPipelineFactory::create(); }))
        .def("init", &GPipeline::init)
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GPipeline)
        .def("setUniqueThreadPoolConfig", &GPipeline::setUniqueThreadPoolConfig,
             py::arg("config"))
        .def("setSharedThreadPool", &GPipeline::setSharedThreadPool,
             py::arg("ptr"),
             py::call_guard<py::gil_scoped_release>(),
             py::keep_alive<1, 2>())
        .def("setGEngineType", &GPipeline::setGEngineType,
             py::arg("type"))
        .def("run", &GPipeline::run,
             py::call_guard<py::gil_scoped_release>())
        .def("process", &GPipeline::process,
             py::arg("runTimes") = 1,
             py::call_guard<py::gil_scoped_release>())
        .def("destroy", &GPipeline::destroy)
        .def("addGEvent", &GPipeline::__addGEvent_4py,
             py::arg("event"),
             py::arg("key"),
             py::keep_alive<1, 2>())
        .def("addGDaemon", &GPipeline::__addGDaemon_4py,
             py::arg("daemon"),
             py::arg("ms"),
             py::keep_alive<1, 2>())
        .def("addGStage", &GPipeline::__addGStage_4py,
             py::arg("stage"),
             py::arg("key"),
             py::arg("threshold"),
             py::keep_alive<1, 2>())
        .def("asyncRun", &GPipeline::asyncRun,
             py::arg("policy") = std::launch::async,
             py::call_guard<py::gil_scoped_release>())
        .def("asyncProcess", &GPipeline::asyncProcess,
             py::arg("runTimes") = CGRAPH_DEFAULT_LOOP_TIMES,
             py::arg("policy") = std::launch::async,
             py::call_guard<py::gil_scoped_release>())
        .def("cancel", &GPipeline::cancel,
            py::call_guard<py::gil_scoped_release>())
        .def("suspend", &GPipeline::suspend,
             py::call_guard<py::gil_scoped_release>())
        .def("resume", &GPipeline::resume,
             py::call_guard<py::gil_scoped_release>())
        .def("perf", &GPipeline::__perf_4py,
             py::call_guard<py::gil_scoped_release>())
        .def("dump", &GPipeline::__dump_4py)
        .def("trim", &GPipeline::trim)
        .def("makeSerial", &GPipeline::makeSerial)
        .def("getMaxPara", &GPipeline::getMaxPara)
        .def("getCurState", &GPipeline::getCurState)
        .def("checkSeparate", &GPipeline::checkSeparate,
             py::arg("fst"),
             py::arg("snd"))
        .def("registerGElement", &GPipeline::__registerGElement_4py,
             py::arg("element"),
             py::arg("depends") = GElementPtrSet{},
             py::arg("name") = CGRAPH_EMPTY,
             py::arg("loop") = CGRAPH_DEFAULT_LOOP_TIMES,
             py::keep_alive<1, 2>());

    py::class_<GPipelineManager>(m, "GPipelineManager")
        .def(py::init<>())
        .def("init", &GPipelineManager::init)
        .def("run", &GPipelineManager::run,
             py::call_guard<py::gil_scoped_release>())
        .def("destroy", &GPipelineManager::destroy)
        .def("add", &GPipelineManager::add,
             py::arg("ptr"),
             py::keep_alive<1, 2>())
        .def("clear", &GPipelineManager::clear)
        .def("find", &GPipelineManager::find,
             py::arg("ptr"))
        .def("remove", &GPipelineManager::remove,
             py::arg("ptr"))
        .def("getSize", &GPipelineManager::getSize)
        .def("fetch", &GPipelineManager::fetch,
             py::call_guard<py::gil_scoped_release>())
        .def("release", &GPipelineManager::release,
             py::arg("ptr"),
             py::call_guard<py::gil_scoped_release>());

    py::class_<GElement, PywGElement, std::unique_ptr<GElement, py::nodelete> >(m, "GElement")
        .def(py::init<>())
        .def("__str__", &GElement::__str_4py)
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GElement)
        .PYCGRAPH_DEF_GEVENT_PYBIND11_FUNCTIONS(GElement)
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
        .def("getLoop", &GElement::getLoop)
        .def("getCurState", &GElement::getCurState)
        .def("addGAspect", &GElement::__addGAspect_4py,
             py::arg("aspect"),
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

    py::class_<GFence, PywGFence, GElement, std::unique_ptr<GFence, py::nodelete> >(m, "GFence")
        .def(py::init<>())
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GFence)
        .def("waitGElement", &GFence::waitGElement,
             py::arg("element"))
        .def("waitGElements", &GFence::waitGElements,
             py::arg("elements"))
        .def("clear", &GFence::clear);

    py::class_<GFunction, PywGFunction, GElement, std::unique_ptr<GFunction, py::nodelete> >(m, "GFunction")
        .def(py::init<>())
        .PYCGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GFunction)
        .PYCGRAPH_DEF_GEVENT_PYBIND11_FUNCTIONS(GFunction)
        .def("setFunction", &GFunction::setFunction,
             py::arg("type"),
             py::arg("func"));

    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GCluster);
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GClusterInterface);

    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GRegion);
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GRegionInterface);

    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GCondition);
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GConditionInterface);

    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GSerialMultiCondition);
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GSerialMultiConditionInterface);

    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GParallelMultiCondition);
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GParallelMultiConditionInterface);

    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GMutable);
    PYCGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GMutableInterface);
}
