/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParam.cpp
@Time: 2023/2/20 22:11
@Desc: 
***************************/

#include "GParam.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GParam::getBacktrace(std::vector<std::string>& traces) {
    CGRAPH_FUNCTION_BEGIN
    if (likely(!backtrace_enable_)) {
        // 非问题排查或信息展示场景，更倾向不开启此功能
        CGRAPH_RETURN_ERROR_STATUS("backtrace no enable.")
    }

    backtrace_lock_.lock();
    traces.clear();
    backtrace_.getUniqueArray(traces);
    backtrace_lock_.unlock();

    CGRAPH_FUNCTION_END
}


CVoid GParam::addBacktrace(const std::string& trace) {
    if (likely(!backtrace_enable_)) {
        // 如果没有开启，直接返回即可
        return;
    }

    // 如果name不为空，则添加name信息。如果name为空，则添加session信息
    backtrace_lock_.lock();
    backtrace_.uniqueAdd(trace);
    backtrace_lock_.unlock();
}


CVoid GParam::cleanBacktrace() {
    if (!backtrace_enable_) {
        return;
    }

    backtrace_lock_.lock();
    backtrace_.clear();
    backtrace_lock_.unlock();
}


const std::string& GParam::getKey() const {
    return key_;
}


CStatus GParam::setup() {
    CGRAPH_EMPTY_FUNCTION
}


CVoid GParam::reset(const CStatus& curStatus) {
}

CGRAPH_NAMESPACE_END