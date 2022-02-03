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

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CFUNCTYPE_H
