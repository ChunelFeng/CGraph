/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParam.h
@Time: 2021/6/11 6:32 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPARAM_H
#define CGRAPH_GPARAM_H

#include <set>
#include <mutex>

#include "GParamObject.h"

CGRAPH_NAMESPACE_BEGIN

class GParam : public GParamObject {
public:
#if __cplusplus >= 201703L && _CGRAPH_GPARAM_RWLOCK_ENABLE_
    std::shared_mutex _param_shared_lock_;    // 用于参数互斥的锁信息
#else
    std::recursive_mutex _param_shared_lock_;
#endif

    /**
     * 获取参数的调用栈信息
     * @return
     */
    std::vector<std::string> getBacktrace();

    /**
     * 添加trace信息
     * @param trace
     * @return
     */
    CStatus addBacktrace(const std::string& trace);

    /**
     * 清空trace信息
     * @return
     */
    CVoid cleanBacktrace();

    /**
     * 获取key信息
     * @return
     */
    std::string getKey() const;

    /**
     * 上锁
     * @return
     * @notice 推荐使用raii锁
     */
    CVoid lock();

    /**
     * 解锁
     * @return
     */
    CVoid unlock();

    /**
     * 尝试加锁
     * @return
     */
    CBool tryLock();

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
     * @notice 为丰富功能和简化操作，v2.4.0 版本中添加了curStatus作为入参，且取消纯虚函数设定。
     */
    virtual CVoid reset(const CStatus& curStatus);


private:
    CBool backtrace_enable_ = false;                             // 是否使能backtrace功能
    std::string key_;                                            // 名称信息
    USerialUniqueArray<std::string> backtrace_;                  // 记录参数的调用栈信息，仅记录get 此参数的地方。不包括 create和remove的地方。
    USpinLock backtrace_lock_;                                   // 针对backtrace的自旋锁

    friend class GParamManager;
    friend class GStorage;
};

using GParamPtr = GParam *;
using GParamPtrSet = std::set<GParamPtr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPARAM_H
