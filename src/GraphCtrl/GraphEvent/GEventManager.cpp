/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEventManager.cpp
@Time: 2023/1/20 23:08
@Desc: 
***************************/

#include "GEventManager.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GEventManager::init() {
    CGRAPH_FUNCTION_BEGIN
    for (auto& iter : events_map_) {
        status += (iter.second)->init();
    }
    CGRAPH_FUNCTION_END
}


CStatus GEventManager::destroy() {
    CGRAPH_FUNCTION_BEGIN
    for (auto& iter : events_map_) {
        status += (iter.second)->destroy();
    }
    CGRAPH_FUNCTION_END
}


CStatus GEventManager::clear() {
    CGRAPH_FUNCTION_BEGIN
    for (auto& iter : events_map_) {
        CGRAPH_DELETE_PTR(iter.second)
    }

    events_map_.clear();
    CGRAPH_FUNCTION_END
}


CStatus GEventManager::trigger(const std::string &key, GEventType type, GEventAsyncStrategy strategy) {
    CGRAPH_FUNCTION_BEGIN
    auto result = events_map_.find(key);
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(events_map_.end() == result,
                                            "event key [" + key + "] no find")

    auto event = result->second;
    CGRAPH_ASSERT_NOT_NULL(event)
    status = event->process(type, strategy);
    CGRAPH_FUNCTION_END
}


std::shared_future<CVoid> GEventManager::asyncTrigger(const std::string &key, GEventAsyncStrategy strategy) {
    auto result = events_map_.find(key);
    CGRAPH_THROW_EXCEPTION_BY_CONDITION(events_map_.end() == result || !result->second,
                                        "event key [" + key + "] no find");

    auto event = result->second;
    return event->asyncProcess(strategy);
}


GEventObjectPtr GEventManager::setThreadPool(UThreadPoolPtr ptr) {
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(ptr)
    for (auto& iter : events_map_) {
        CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(iter.second)
        (iter.second)->setThreadPool(ptr);
    }

    return this;
}


GEventManager::~GEventManager() {
    clear();
}


CStatus GEventManager::reset() {
    CGRAPH_FUNCTION_BEGIN
    for (auto& iter : events_map_) {
        iter.second->asyncWait(GEventAsyncStrategy::PIPELINE_RUN_FINISH);
    }
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END