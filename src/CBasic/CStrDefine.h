/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CInfoDefine.h
@Time: 2022/4/16 14:01
@Desc: 
***************************/

#ifndef CGRAPH_CSTRDEFINE_H
#define CGRAPH_CSTRDEFINE_H

#include <string>

#include "CBasicDefine.h"

CGRAPH_NAMESPACE_BEGIN

static const std::string& CGRAPH_EMPTY = "";
static const std::string& CGRAPH_DEFAULT = "default";
static const std::string& CGRAPH_UNKNOWN = "unknown";
static const std::string& CGRAPH_BASIC_EXCEPTION = "CGraph default exception";
static const std::string& CGRAPH_FUNCTION_NO_SUPPORT = "CGraph function no support";
static const std::string& CGRAPH_INPUT_IS_NULL = "input is nullptr";

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CSTRDEFINE_H
