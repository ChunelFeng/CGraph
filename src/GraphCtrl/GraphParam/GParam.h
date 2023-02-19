/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParam.h
@Time: 2021/6/11 6:32 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPARAM_H
#define CGRAPH_GPARAM_H

#include <string>
#include <set>

    #if __cplusplus >= 201703L
#include <shared_mutex>
    #else
#include <mutex>
    #endif

#include "GParamObject.h"

CGRAPH_NAMESPACE_BEGIN

class GParam : public GParamObject {
public:
#if __cplusplus >= 201703L
    std::shared_mutex _param_shared_lock_;    // 用于参数互斥的锁信息
#else
    std::mutex _param_shared_lock_;
#endif
    /**
     * 设置是否开启backtrace功能，默认关闭
     * @param enable
     * @return
     */
    GParam* setTracebackEnable(CBool enable) {
        backtrace_enable_ = enable;
        return this;
    }

    /**
     * 获取参数的调用栈信息
     * @param backtrace
     * @return
     */
    CStatus getBacktrace(std::set<std::string>& backtrace) {
        CGRAPH_FUNCTION_BEGIN
        if (!backtrace_enable_) {
            CGRAPH_RETURN_ERROR_STATUS("backtrace no enable.");
        }

        CGRAPH_READ_LOCK lk(_param_shared_lock_);
        for (const auto& iter : backtrace_) {
            backtrace.emplace(iter);
        }

        CGRAPH_FUNCTION_END
    }

    /**
     * 添加回溯信息
     * @param name
     * @param session
     * @return
     */
    CVoid addBacktrace(const std::string& name, const std::string& session) {
        if (!backtrace_enable_) {
            // 如果没有开启，直接返回即可
            return;
        }

        // 如果name不为空，则添加name信息。如果name为空，则添加session信息
        CGRAPH_WRITE_LOCK lk(_param_shared_lock_);
        backtrace_.insert(name.empty() ? session : name);
    }

    /**
     * 保存参数信息
     * @param path 位置路径
     * @return
     */
    virtual CStatus dump(const std::string& path) {
        CGRAPH_NO_SUPPORT
    }

    /**
     * 加载参数信息
     * @param path 位置路径
     * @return
     */
    virtual CStatus load(const std::string& path) {
        CGRAPH_NO_SUPPORT
    }

protected:
    /**
     * 每次pipeline执行结束，会调用一次reset
     * @return
     */
    virtual CVoid reset() = 0;

    /**
     * 每次pipeline执行前，会调用一次setup，可以不实现
     * @return
     */
    virtual CStatus setup() {
        CGRAPH_EMPTY_FUNCTION
    }

private:
    std::set<std::string> backtrace_;        // 记录参数的调用栈信息
    CBool backtrace_enable_ = false;         // 是否使能backtrace功能

    friend class GParamManager;
};

using GParamPtr = GParam *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPARAM_H
