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
    for (auto iter : events_map_) {
        status += (iter.second)->init();
    }
    CGRAPH_FUNCTION_END
}


CStatus GEventManager::destroy() {
    CGRAPH_FUNCTION_BEGIN
    for (auto iter : events_map_) {
        status += (iter.second)->destroy();
    }
    CGRAPH_FUNCTION_END
}


CStatus GEventManager::clear() {
    CGRAPH_FUNCTION_BEGIN
    for (auto iter : events_map_) {
        CGRAPH_DELETE_PTR(iter.second)
    }

    events_map_.clear();
    CGRAPH_FUNCTION_END
}


CStatus GEventManager::trigger(const std::string &key, GEventType type) {
    CGRAPH_FUNCTION_BEGIN
    auto result = events_map_.find(key);
    if (events_map_.end() == result) {
        CGRAPH_RETURN_ERROR_STATUS("event key [" + key + "] no find")
    }

    auto event = result->second;
    CGRAPH_ASSERT_NOT_NULL(event)
    status = event->process(type);
    CGRAPH_FUNCTION_END
}


GEventObject* GEventManager::setThreadPool(UThreadPoolPtr ptr) {
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

CGRAPH_NAMESPACE_END