/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParam.cpp
@Time: 2023/2/20 22:11
@Desc: 
***************************/

#include "GParam.h"

CGRAPH_NAMESPACE_BEGIN


CStatus GParam::dump(const std::string& path) {
    CGRAPH_NO_SUPPORT
}


CStatus GParam::load(const std::string& path) {
    CGRAPH_NO_SUPPORT
}


CStatus GParam::getBacktrace(std::set<std::string>& backtrace) {
    CGRAPH_FUNCTION_BEGIN
    if (!backtrace_enable_) {
        CGRAPH_RETURN_ERROR_STATUS("backtrace no enable.")
    }

    CGRAPH_READ_LOCK lk(_param_shared_lock_);
    for (const auto& iter : backtrace_) {
        backtrace.emplace(iter);
    }

    CGRAPH_FUNCTION_END
}


CVoid GParam::addBacktrace(const std::string& name, const std::string& session) {
    if (!backtrace_enable_) {
        // 如果没有开启，直接返回即可
        return;
    }

    // 如果name不为空，则添加name信息。如果name为空，则添加session信息
    CGRAPH_WRITE_LOCK lk(_param_shared_lock_);
    backtrace_.insert(name.empty() ? session : name);
}


const std::string& GParam::getKey() const {
    return key_;
}

CGRAPH_NAMESPACE_END