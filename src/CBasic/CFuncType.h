/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CFuncType.h
@Time: 2022/2/3 1:05 下午
@Desc: 
***************************/

#ifndef CGRAPH_CFUNCTYPE_H
#define CGRAPH_CFUNCTYPE_H

#include <functional>

#include "CValType.h"

CGRAPH_NAMESPACE_BEGIN

using CGRAPH_DEFAULT_FUNCTION = std::function<void()>;
using CGRAPH_DEFAULT_CONST_FUNCTION_REF = const std::function<void()>&;
using CGRAPH_CSTATUS_FUNCTION = std::function<CStatus()>;
using CGRAPH_CSTATUS_CONST_FUNCTION_REF = const std::function<CStatus()>&;
using CGRAPH_CALLBACK_FUNCTION = std::function<void(CStatus)>;
using CGRAPH_CALLBACK_CONST_FUNCTION_REF = const std::function<void(CStatus)>&;

/** 开启函数流程 */
#define CGRAPH_FUNCTION_BEGIN                                           \
    CStatus status;                                                     \

/** 结束函数流程 */
#define CGRAPH_FUNCTION_END                                             \
    return status;                                                      \

/** 无任何功能函数 */
#define CGRAPH_EMPTY_FUNCTION                                           \
    return CStatus();                                                   \

/** 不支持当前功能 */
#define CGRAPH_NO_SUPPORT                                               \
    return CStatus("function not support");                             \

#define CGRAPH_RETURN_ERROR_STATUS(info)                                \
    return CStatus(info);                                               \


/* 定义为不能赋值和拷贝的对象 */
#define CGRAPH_NO_ALLOWED_COPY(CType)                                   \
    CType(const CType &) = delete;                                      \
    const CType &operator=(const CType &) = delete;                     \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CFUNCTYPE_H
