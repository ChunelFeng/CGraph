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

#include "CInfoDefine.h"
#include "CValType.h"

CGRAPH_NAMESPACE_BEGIN

using CGRAPH_DEFAULT_FUNCTION = std::function<void()>;
using CGRAPH_DEFAULT_CONST_FUNCTION_REF = const std::function<void()>&;
using CGRAPH_CSTATUS_FUNCTION = std::function<CStatus()>;
using CGRAPH_CSTATUS_CONST_FUNCTION_REF = const std::function<CStatus()>&;
using CGRAPH_CALLBACK_FUNCTION = std::function<void(CStatus)>;
using CGRAPH_CALLBACK_CONST_FUNCTION_REF = const std::function<void(CStatus)>&;


/**
 * 描述函数类型
 */
enum class CFunctionType {
    INIT = 1,              /** 初始化函数 */
    RUN = 2,               /** 执行函数 */
    DESTROY = 3            /** 释放函数 */
};

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
    return CStatus(CGRAPH_FUNCTION_NO_SUPPORT);                         \

/** 返回异常信息和状态 */
#define CGRAPH_RETURN_ERROR_STATUS(info)                                \
    return CStatus(info);                                               \

/** 定义为不能赋值和拷贝的对象类型 */
#define CGRAPH_NO_ALLOWED_COPY(CType)                                   \
    CType(const CType &) = delete;                                      \
    const CType &operator=(const CType &) = delete;                     \

/** 抛出异常 */
#define CGRAPH_THROW_EXCEPTION(info)                                    \
    throw CException(info);                                             \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CFUNCTYPE_H
