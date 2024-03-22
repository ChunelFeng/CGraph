/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GDaemonManager.cpp
@Time: 2022/2/3 10:26 上午
@Desc: 
***************************/

#include "GDaemonManager.h"

CGRAPH_NAMESPACE_BEGIN

GDaemonManager::~GDaemonManager() {
    clear();
}


CStatus GDaemonManager::init() {
    CGRAPH_FUNCTION_BEGIN
    for (auto daemon: daemons_) {
        CGRAPH_ASSERT_NOT_NULL(daemon)
        status += daemon->init();
    }

    CGRAPH_FUNCTION_END
}


CStatus GDaemonManager::destroy() {
    CGRAPH_FUNCTION_BEGIN
    for (auto daemon: daemons_) {
        CGRAPH_ASSERT_NOT_NULL(daemon)
        status += daemon->destroy();
    }

    CGRAPH_FUNCTION_END
}


CStatus GDaemonManager::add(GDaemonPtr daemon) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(daemon)

    daemons_.insert(daemon);
    CGRAPH_FUNCTION_END
}


CStatus GDaemonManager::remove(GDaemonPtr daemon) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(daemon)

    daemons_.erase(daemon);
    CGRAPH_DELETE_PTR(daemon)
    CGRAPH_FUNCTION_END
}


CStatus GDaemonManager::clear() {
    CGRAPH_FUNCTION_BEGIN
    for (auto daemon: daemons_) {
        CGRAPH_DELETE_PTR(daemon)
    }

    daemons_.clear();
    CGRAPH_FUNCTION_END
}


CSize GDaemonManager::getSize() const {
    return daemons_.size();
}

CGRAPH_NAMESPACE_END
