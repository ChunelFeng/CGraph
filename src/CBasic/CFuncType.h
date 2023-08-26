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

#include "CStrDefine.h"
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


/** 获取当前代码所在的位置信息 */
#define CGRAPH_GET_LOCATE                                               \
    (std::string(__FILE__) + " | " + std::string(__FUNCTION__)          \
    + " | line = [" + ::std::to_string( __LINE__) + "]")


/** 生成一个包含异常位置的 CStatus
 * 这里这样实现，是为了符合 CStatus 类似写法
 * */
#define CErrStatus(info)                                                \
    CStatus(info, CGRAPH_GET_LOCATE)                                    \

/** 返回异常信息和状态 */
#define CGRAPH_RETURN_ERROR_STATUS(info)                                \
    return CErrStatus(info);                                            \

/** 根据条件判断是否返回错误状态 */
#define CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(cond, info)             \
    if (unlikely(cond)) { CGRAPH_RETURN_ERROR_STATUS(info); }           \

/** 不支持当前功能 */
#define CGRAPH_NO_SUPPORT                                               \
    return CErrStatus(CGRAPH_FUNCTION_NO_SUPPORT);                      \

/** 定义为不能赋值和拷贝的对象类型 */
#define CGRAPH_NO_ALLOWED_COPY(CType)                                   \
    CType(const CType &) = delete;                                      \
    const CType &operator=(const CType &) = delete;                     \

/** 抛出异常 */
#define CGRAPH_THROW_EXCEPTION(info)                                    \
    throw CException(info, CGRAPH_GET_LOCATE);                          \

/** 在异常状态的情况下，抛出异常 */
#define CGRAPH_THROW_EXCEPTION_BY_STATUS(status)                        \
    if (unlikely((status).isErr())) {                                   \
        CGRAPH_THROW_EXCEPTION((status).getInfo());                     \
    }                                                                   \

/** 根据条件判断是否抛出异常 */
#define CGRAPH_THROW_EXCEPTION_BY_CONDITION(cond, info)                 \
    if (unlikely(cond)) { CGRAPH_THROW_EXCEPTION(info); }               \


CGRAPH_NAMESPACE_END

#endif //CGRAPH_CFUNCTYPE_H
