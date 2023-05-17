/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParamUtils.h
@Time: 2022/11/1 23:37
@Desc:
***************************/

#ifndef CGRAPH_GPARAMUTILS_H
#define CGRAPH_GPARAMUTILS_H

#include "../GraphObject.h"

CGRAPH_NAMESPACE_BEGIN

/** 创建参数信息 */
#define CGRAPH_CREATE_GPARAM(Type, key)                                       \
    this->template createGParam<Type>(key);                                   \

#define CGRAPH_CREATE_GPARAM_WITH_BACKTRACE(Type, key)                        \
    this->template createGParam<Type>(key, true);                             \

/** 获取参数信息 */
#define CGRAPH_GET_GPARAM(Type, key)                                          \
    this->template getGParam<Type>(key);                                      \

/** 获取参数信息，为空则抛出异常 */
#define CGRAPH_GET_GPARAM_WITH_NO_EMPTY(Type, key)                            \
    this->template getGParamWithNoEmpty<Type>(key);                           \

/** 获取参数的GParam的keys信息 */
#define CGRAPH_GET_GPARAM_KEYS(keys)                                          \
    this->template getGParamKeys(keys);                                       \

/** 删除一个参数 */
#define CGRAPH_DELETE_GPARAM(key)                                             \
    this->removeGParam(key);                                                  \

/** 获取element内部参数信息 */
#define CGRAPH_GET_EPARAM(Type, key)                                          \
    this->template getEParam<Type>(key);                                      \

/** 上参数写锁 */
#define CGRAPH_PARAM_WRITE_CODE_BLOCK(param)                                  \
    CGraph::CGRAPH_WRITE_LOCK __paramWLock__((param)->_param_shared_lock_);   \

/** 上参数读锁 */
#define CGRAPH_PARAM_READ_CODE_BLOCK(param)                                   \
    CGraph::CGRAPH_READ_LOCK __paramRLock__((param)->_param_shared_lock_);    \

CGRAPH_NAMESPACE_END


#endif //CGRAPH_GPARAMUTILS_H
