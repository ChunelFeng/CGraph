/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParam.cpp
@Time: 2023/2/20 22:11
@Desc: 
***************************/

#include "GParam.h"

CGRAPH_NAMESPACE_BEGIN

std::vector<std::string> GParam::getBacktrace() {
    CGRAPH_THROW_EXCEPTION_BY_CONDITION(!backtrace_enable_,    \
                                        "no enable backtrace for [" + key_ + "] param")

    std::vector<std::string> traces;
    backtrace_lock_.lock();
    backtrace_.getUniqueArray(traces);
    backtrace_lock_.unlock();

    return traces;
}


CStatus GParam::addBacktrace(const std::string& trace) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!backtrace_enable_,    \
                                            "no enable backtrace for [" + key_ + "] param")

    // 如果name不为空，则添加name信息。如果name为空，则添加session信息
    backtrace_lock_.lock();
    backtrace_.uniqueAdd(trace);
    backtrace_lock_.unlock();

    CGRAPH_FUNCTION_END
}


CVoid GParam::cleanBacktrace() {
    if (!backtrace_enable_) {
        return;
    }

    backtrace_lock_.lock();
    backtrace_.clear();
    backtrace_lock_.unlock();
}


std::string GParam::getKey() const {
    return key_;
}


CStatus GParam::setup() {
    CGRAPH_EMPTY_FUNCTION
}


CVoid GParam::reset(const CStatus& curStatus) {
}


CVoid GParam::lock() {
    _param_shared_lock_.lock();
}


CVoid GParam::unlock() {
    _param_shared_lock_.unlock();
}


CBool GParam::tryLock() {
    return _param_shared_lock_.try_lock();
}

CGRAPH_NAMESPACE_END