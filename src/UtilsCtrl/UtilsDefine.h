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
#include <shared_mutex>
#include <ctime>
#ifndef _WIN32
    #include <sys/timeb.h>
#endif

#include "../CObject/CObject.h"

static std::mutex g_check_status_mtx;
static std::mutex g_echo_mtx;

inline void CGRAPH_ECHO(const char *cmd, ...) {
#ifdef _CGRAPH_SILENCE_
    return;
#endif

    std::lock_guard<std::mutex> lock{ g_echo_mtx };
#ifndef _WIN32
    // 非windows系统，打印到毫秒
    timeb cur_time{};
    char timeInfo[32] = {0};

    ftime(&cur_time);
    tm *ptm = localtime(&cur_time.time);
    sprintf(timeInfo, " [%04d-%02d-%02d %02d:%02d:%02d.%03d] ",
            ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday,
            ptm->tm_hour, ptm->tm_min, ptm->tm_sec, cur_time.millitm);
    std::cout << "[CGraph]" << timeInfo;
#else
    // windows系统，打印到秒
    time_t cur_time = time(nullptr);
        std::string ct = ctime(&cur_time);
        std::cout << "[cgraph] ["
                  << ct.assign(ct.begin(), ct.tail()-1)    // 去掉时间的最后一位\n信息
                  << "] ";
#endif

    va_list args;
    va_start(args, cmd);
    vprintf(cmd, args);
    va_end(args);
    std::cout << "" << std::endl;
}

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

typedef std::shared_lock<std::shared_mutex> CGRAPH_RLOCK;
typedef std::unique_lock<std::shared_mutex> CGRAPH_WLOCK;

/* 开启函数流程 */
#define CGRAPH_FUNCTION_BEGIN                       \
    CSTATUS status = STATUS_OK;                     \

/* 结束函数流程 */
#define CGRAPH_FUNCTION_END                         \
    return status;                                  \

/* 判断传入的指针信息是否为空 */
#define CGRAPH_ASSERT_NOT_NULL(ptr)                 \
    if (nullptr == (ptr)) {                         \
        return STATUS_RES;                          \
    }                                               \

#define CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(ptr)     \
    if (nullptr == (ptr)) {                         \
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
    if ((ptr) != nullptr) {                         \
        delete (ptr);                               \
        (ptr) = nullptr;                            \
    }                                               \

#define CGRAPH_ASSERT_INIT(isInit)                  \
    if (isInit != is_init_) {                       \
        return STATUS_ERR;                          \
    }                                               \

#define CGRAPH_NODE_SAFE_PROCESS(node)              \
    CGRAPH_ASSERT_NOT_NULL(node)                    \
    status = node->process();                       \
    CGRAPH_FUNCTION_CHECK_STATUS                    \

#define CGRAPH_NO_SUPPORT                           \
    return STATUS_ERR;                              \


#endif //CGRAPH_UTILSDEFINE_H
