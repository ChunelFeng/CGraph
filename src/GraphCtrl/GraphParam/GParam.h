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
     * 获取参数的调用栈信息
     * @param trace
     * @return
     */
    CStatus getBacktrace(std::vector<std::string>& trace);

    /**
     * 添加trace信息
     * @param trace
     * @return
     */
    CVoid addBacktrace(const std::string& trace);

    /**
     * 清空trace信息
     * @return
     */
    CVoid cleanBacktrace();

    /**
     * 获取对应的key信息
     * @return
     */
    const std::string& getKey() const;

protected:
    /**
     * 每次pipeline执行前，会调用一次setup，可以不实现
     * @return
     */
    virtual CStatus setup();

    /**
     * 每次pipeline执行结束，会调用一次reset
     * @param curStatus 本次pipeline执行的状态
     * @return
     * @notice 为丰富功能，v2.3.4 版本中添加了curStatus作为入参，不兼容之前逻辑。造成的困扰，表示抱歉
     */
    virtual CVoid reset(const CStatus& curStatus) = 0;


private:
    std::string key_;                                            // 对应的key信息
    USerialUniqueArray<std::string> backtrace_;                  // 记录参数的调用栈信息，仅记录get 此参数的地方。不包括 create和remove的地方。
    CBool backtrace_enable_ = false;                             // 是否使能backtrace功能
    USpinLock backtrace_lock_;                                   // 针对backtrace的自旋锁

    friend class GParamManager;
};

using GParamPtr = GParam *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPARAM_H
