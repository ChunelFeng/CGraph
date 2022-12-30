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

/* 判断传入的指针信息是否为空 */
#define CGRAPH_ASSERT_NOT_NULL(ptr)                 \
    if (unlikely(nullptr == (ptr))) {               \
        return CStatus("input is nullptr");         \
    }                                               \

#define CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(ptr)     \
    if (unlikely(nullptr == (ptr))) {               \
        return nullptr;                             \
    }                                               \

#define CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(ptr)     \
    if (unlikely(nullptr == (ptr))) {               \
        CGRAPH_THROW_EXCEPTION("input is null")     \
    }


/* 判断函数流程是否可以继续 */
static std::mutex g_check_status_mtx;
#define CGRAPH_FUNCTION_CHECK_STATUS                                                         \
    if (unlikely(!status.isOK())) {                                                          \
        std::lock_guard<std::mutex> lock{ g_check_status_mtx };                              \
        CGRAPH_ECHO("%s | %s | line = [%d], errorCode = [%d], errorInfo = [%s].",            \
            __FILE__, __FUNCTION__, __LINE__, status.getCode(), status.getInfo().c_str());   \
        return status;                                                                       \
    }                                                                                        \

/* 删除资源信息 */
#define CGRAPH_DELETE_PTR(ptr)                                \
    if (unlikely((ptr) != nullptr)) {                         \
        delete (ptr);                                         \
        (ptr) = nullptr;                                      \
    }                                                         \

#define CGRAPH_ASSERT_INIT(isInit)                            \
    if (unlikely((isInit) != is_init_)) {                     \
        return CStatus("init status is not suitable");        \
    }                                                         \

#define CGRAPH_ASSERT_INIT_RETURN_NULL(isInit)                \
    if (unlikely((isInit) != is_init_)) {                     \
        return nullptr;                                       \
    }                                                         \

#define CGRAPH_SLEEP_MILLISECOND(ms)                                            \
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));                 \

#define CGRAPH_SLEEP_SECOND(s)                                                  \
    std::this_thread::sleep_for(std::chrono::seconds(s));                       \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTILSDEFINE_H
