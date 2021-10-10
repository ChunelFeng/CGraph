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
#include <ctime>
#include <cstdarg>

#ifdef _GENERATE_SESSION_
    #include <uuid/uuid.h>
#endif

#include "../CObject/CObject.h"
#include "ThreadPool/UThreadPoolDefine.h"

CGRAPH_NAMESPACE_BEGIN

static std::mutex g_check_status_mtx;
static std::mutex g_echo_mtx;
static std::mutex g_session_mtx;

/* CGRAPH 个性化输出 */
inline void CGRAPH_ECHO(const char *cmd, ...) {
#ifdef _CGRAPH_SILENCE_
    return;
#endif

    CGRAPH_LOCK_GUARD lock{ g_echo_mtx };
#ifndef _WIN32
    // 非windows系统，打印到毫秒
    auto now = std::chrono::system_clock::now();
    //通过不同精度获取相差的毫秒数
    uint64_t disMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
                      - std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;
    time_t tt = std::chrono::system_clock::to_time_t(now);
    auto time_tm = localtime(&tt);
    char strTime[32] = { 0 };
    sprintf(strTime, " [%d-%02d-%02d %02d:%02d:%02d.%03d]", time_tm->tm_year + 1900,
            time_tm->tm_mon + 1, time_tm->tm_mday, time_tm->tm_hour,
            time_tm->tm_min, time_tm->tm_sec, (int)disMs);
    std::cout << "[CGraph]" << strTime << " ";
#else
    // windows系统，打印到秒
    time_t cur_time = time(nullptr);
        std::string ct = ctime(&cur_time);
        std::cout << "[cgraph] ["
                  << ct.assign(ct.begin(), ct.end()-1)    // 去掉时间的最后一位\n信息
                  << "] ";
#endif

    va_list args;
    va_start(args, cmd);
    vprintf(cmd, args);
    va_end(args);
    std::cout << "\n";
}

#ifdef _ENABLE_LIKELY_
    #define likely(x)   __builtin_expect(!!(x), 1)
    #define unlikely(x) __builtin_expect(!!(x), 0)
#else
    #define likely
    #define unlikely
#endif


template<typename T>
inline T* SafeMallocCObject() {
    T* ptr = nullptr;
    while (!ptr && std::is_base_of_v<CObject, T>) {
        ptr = new(std::nothrow) T();
    }
    return ptr;
}


inline std::string CGRAPH_GENERATE_SESSION() {
#ifdef _GENERATE_SESSION_
    CGRAPH_LOCK_GUARD lock{ g_session_mtx };
    uuid_t uuid;
    char session[36] = {0};    // 36是特定值
    uuid_generate(uuid);
    uuid_unparse(uuid, session);

    return session;
#else
    return "";    // 非mac平台，暂时不支持自动生成session信息
#endif
}


/* 创建CObject对象 */
#define CGRAPH_SAFE_MALLOC_COBJECT(TYPE)             \
    SafeMallocCObject<TYPE>();                       \

/* 开启函数流程 */
#define CGRAPH_FUNCTION_BEGIN                       \
    CSTATUS status = STATUS_OK;                     \

/* 结束函数流程 */
#define CGRAPH_FUNCTION_END                         \
    return status;                                  \

/* 判断传入的指针信息是否为空 */
#define CGRAPH_ASSERT_NOT_NULL(ptr)                 \
    if (unlikely(nullptr == (ptr))) {               \
        return STATUS_RES;                          \
    }                                               \

#define CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(ptr)     \
    if (unlikely(nullptr == (ptr))) {               \
        return nullptr;                             \
    }                                               \


/* 判断函数流程是否可以继续 */
#define CGRAPH_FUNCTION_CHECK_STATUS                                                                       \
    if (unlikely(STATUS_OK != status)) {                                                                   \
        std::lock_guard<std::mutex> lock{ g_check_status_mtx };                                            \
        CGRAPH_ECHO("%s | %s | line = [%d], status = [%d].", __FILE__, __FUNCTION__, __LINE__, status);    \
        return status;                                                                                     \
    }                                                                                                      \

/* 删除资源信息 */
#define CGRAPH_DELETE_PTR(ptr)                      \
    if (unlikely((ptr) != nullptr)) {               \
        delete (ptr);                               \
        (ptr) = nullptr;                            \
    }                                               \

#define CGRAPH_ASSERT_INIT(isInit)                  \
    if (unlikely((isInit) != is_init_)) {           \
        return STATUS_ERR;                          \
    }                                               \

#define CGRAPH_NO_SUPPORT                           \
    return STATUS_ERR;                              \

#define CGRAPH_SLEEP_MILLISECOND(ms)                                    \
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));         \

#define CGRAPH_SLEEP_SECOND(s)                                          \
    std::this_thread::sleep_for(std::chrono::seconds(s));               \

#define CGRAPH_PARAM_WRITE_CODE_BLOCK(param)                            \
    CGRAPH_WRITE_LOCK __paramWLock__((param)->_param_shared_lock_);     \

#define CGRAPH_PARAM_READ_CODE_BLOCK(param)                             \
    CGRAPH_READ_LOCK __paramRLock__((param)->_param_shared_lock_);      \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTILSDEFINE_H
