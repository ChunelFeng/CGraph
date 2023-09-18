/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UtilsDefine.h
@Time: 2021/4/30 8:52 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTILSDEFINE_H
#define CGRAPH_UTILSDEFINE_H

#include <iostream>
#include <string>

    #if __cplusplus >= 201703L
#include <shared_mutex>
    #else
#include <mutex>
    #endif

#include "../CBasic/CBasicInclude.h"
#include "UAllocator.h"
#include "UtilsFunction.h"

CGRAPH_NAMESPACE_BEGIN

#ifdef _ENABLE_LIKELY_
    #define likely(x)   __builtin_expect(!!(x), 1)
    #define unlikely(x) __builtin_expect(!!(x), 0)
#else
    #define likely
    #define unlikely
#endif

using CGRAPH_LOCK_GUARD = std::lock_guard<std::mutex>;
using CGRAPH_UNIQUE_LOCK = std::unique_lock<std::mutex>;

#if __cplusplus >= 201703L
    using CGRAPH_READ_LOCK = std::shared_lock<std::shared_mutex>;
    using CGRAPH_WRITE_LOCK = std::unique_lock<std::shared_mutex>;
#else
    using CGRAPH_READ_LOCK = CGRAPH_LOCK_GUARD;    // C++14不支持读写锁，使用mutex替代
    using CGRAPH_WRITE_LOCK = CGRAPH_LOCK_GUARD;
#endif


template<typename T>
CStatus __ASSERT_NOT_NULL(T t) {
    return (unlikely(nullptr == t))
           ? CErrStatus(CGRAPH_INPUT_IS_NULL)
           : CStatus();
}

template<typename T, typename... Args>
CStatus __ASSERT_NOT_NULL(T t, Args... args) {
    if (unlikely(t == nullptr)) {
        return __ASSERT_NOT_NULL(t);
    }

    return __ASSERT_NOT_NULL(args...);
}

template<typename T>
CVoid __ASSERT_NOT_NULL_THROW_EXCEPTION(T t) {
    if (unlikely(nullptr == t)) {
        CGRAPH_THROW_EXCEPTION("[CException] " + CGRAPH_INPUT_IS_NULL)
    }
}

template<typename T, typename... Args>
CVoid __ASSERT_NOT_NULL_THROW_EXCEPTION(T t, Args... args) {
    if (unlikely(nullptr == t)) {
        __ASSERT_NOT_NULL_THROW_EXCEPTION(t);
    }

    __ASSERT_NOT_NULL_THROW_EXCEPTION(args...);
}


/** 判断传入的多个指针信息，是否为空 */
#define CGRAPH_ASSERT_NOT_NULL(ptr, ...)                                                     \
    {                                                                                        \
        const CStatus& __cur_status__ = __ASSERT_NOT_NULL(ptr, ##__VA_ARGS__);               \
        if (unlikely(__cur_status__.isErr())) { return __cur_status__; }                     \
    }                                                                                        \


/** 判断传入的多个指针，是否为空。如果为空，则抛出异常信息 */
#define CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(ptr, ...)                                         \
    __ASSERT_NOT_NULL_THROW_EXCEPTION(ptr, ##__VA_ARGS__);                                   \


/* 判断函数流程是否可以继续 */
static std::mutex g_check_status_mtx;
#define CGRAPH_FUNCTION_CHECK_STATUS                                                         \
    if (unlikely(status.isErr())) {                                                          \
        if (status.isCrash()) { throw CException(status.getInfo()); }                        \
        CGRAPH_LOCK_GUARD lock{ g_check_status_mtx };                                        \
        CGRAPH_ECHO("%s, errorCode = [%d], errorInfo = [%s].",                               \
            status.getLocate().c_str(), status.getCode(), status.getInfo().c_str());         \
        return status;                                                                       \
    }                                                                                        \

/* 删除资源信息 */
#define CGRAPH_DELETE_PTR(ptr)                                                  \
    if (unlikely((ptr) != nullptr)) {                                           \
        delete (ptr);                                                           \
        (ptr) = nullptr;                                                        \
    }                                                                           \

#define CGRAPH_ASSERT_INIT(isInit)                                              \
    if (unlikely((isInit) != is_init_)) {                                       \
        CGRAPH_RETURN_ERROR_STATUS("init status is not suitable")               \
    }                                                                           \

#define CGRAPH_ASSERT_INIT_THROW_ERROR(isInit)                                  \
    if (unlikely((isInit) != is_init_)) {                                       \
        CGRAPH_THROW_EXCEPTION("[CException] init status is not suitable")      \
    }                                                                           \


#define CGRAPH_CHECK_STATUS_RETURN_THIS_OR_NULL                                 \
    return status.isOK() ? this : nullptr;                                      \

#define CGRAPH_SLEEP_SECOND(s)                                                  \
    std::this_thread::sleep_for(std::chrono::seconds(s));                       \

#define CGRAPH_SLEEP_MILLISECOND(ms)                                            \
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));                 \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTILSDEFINE_H
