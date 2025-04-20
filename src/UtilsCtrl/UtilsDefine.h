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
#include <type_traits>

    #if __cplusplus >= 201703L
#include <shared_mutex>
    #else
#include <mutex>
    #endif

#include "../CBasic/CBasicInclude.h"
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

#if __cplusplus >= 201703L && _CGRAPH_GPARAM_RWLOCK_ENABLE_
    using CGRAPH_READ_LOCK = std::shared_lock<std::shared_mutex>;
    using CGRAPH_WRITE_LOCK = std::unique_lock<std::shared_mutex>;
#else
    using CGRAPH_READ_LOCK = std::lock_guard<std::recursive_mutex>;    // C++11和14不支持读写锁，使用 recursive_mutex 替代
    using CGRAPH_WRITE_LOCK = std::lock_guard<std::recursive_mutex>;
#endif


#define __IS_NULLPTR(t) (unlikely(std::is_pointer<decltype(t)>::value && !(t)))

template<typename T>
CStatus __ASSERT_NOT_NULL(T t) {
    return __IS_NULLPTR(t) ? CStatus(CGRAPH_INPUT_IS_NULL) : CStatus();
}

template<typename T, typename... Args>
CStatus __ASSERT_NOT_NULL(T t, Args... args) {
    if (__IS_NULLPTR(t)) {
        return __ASSERT_NOT_NULL(t);
    }

    return __ASSERT_NOT_NULL(args...);
}

template<typename T>
CVoid __ASSERT_NOT_NULL_THROW_EXCEPTION(T t) {
    if (__IS_NULLPTR(t)) {
        CGRAPH_THROW_EXCEPTION("[CException] " + std::string(CGRAPH_INPUT_IS_NULL))
    }
}

template<typename T, typename... Args>
CVoid __ASSERT_NOT_NULL_THROW_EXCEPTION(T t, Args... args) {
    if (__IS_NULLPTR(t)) {
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

/* 删除资源信息 */
#define CGRAPH_DELETE_PTR(ptr)                                                  \
    if (unlikely((ptr) != nullptr)) {                                           \
        delete (ptr);                                                           \
        (ptr) = nullptr;                                                        \
    }                                                                           \

#define CGRAPH_DELETE_PTR_ARRAY(arr)                                            \
    if (unlikely((arr) != nullptr)) {                                           \
        delete[] (arr);                                                         \
        (arr) = nullptr;                                                        \
    }                                                                           \
                                                                                \

#define CGRAPH_ASSERT_INIT(isInit)                                              \
    if (unlikely((isInit) != is_init_)) {                                       \
        CGRAPH_RETURN_ERROR_STATUS("init status is not suitable")               \
    }                                                                           \

#define CGRAPH_ASSERT_INIT_THROW_ERROR(isInit)                                  \
    if (unlikely((isInit) != is_init_)) {                                       \
        CGRAPH_THROW_EXCEPTION("[CException] init status is not suitable") }    \

#define CGRAPH_ASSERT_MUTABLE_INIT_THROW_ERROR(isInit)                                  \
    if (unlikely((isInit) != is_init_) && !isMutable()) {                               \
        CGRAPH_THROW_EXCEPTION("[CException] mutable init status is not suitable") }    \

#define CGRAPH_SLEEP_SECOND(s)                                                  \
    std::this_thread::sleep_for(std::chrono::seconds(s));                       \

#define CGRAPH_SLEEP_MILLISECOND(ms)                                            \
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));                 \

#define CGRAPH_FUNCTION_CHECK_STATUS                                            \
    if (unlikely(status.isErr())) {                                             \
        if (status.isCrash()) { throw CException(status.getInfo()); }           \
        CGRAPH_ECHO("errorCode = [%d], errorInfo = [%s].",                      \
            status.getCode(), status.getInfo().c_str());                        \
        return status;                                                          \
    }                                                                           \

/**
* 定制化输出
* @param cmd
* @param ...
* 注：内部包含全局锁，不建议正式上线的时候使用
*/
inline CVoid CGRAPH_ECHO(const char *cmd, ...) {
#ifdef _CGRAPH_SILENCE_
    return;
#endif

    static std::mutex echo_mtx;
    std::lock_guard<std::mutex> lock{ echo_mtx };
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
    std::cout << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S.")    \
    << std::setfill('0') << std::setw(3) << ms << "] ";

    va_list args;
    va_start(args, cmd);
    vprintf(cmd, args);
    va_end(args);
    std::cout << "\n";
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTILSDEFINE_H
