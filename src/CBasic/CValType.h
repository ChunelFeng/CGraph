/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CValTypes.h
@Time: 2022/2/3 12:58 下午
@Desc: 
***************************/

#ifndef CGRAPH_CVALTYPE_H
#define CGRAPH_CVALTYPE_H

#include "CBasicDefine.h"
#include "CStatus.h"
#include "CException.h"

using CChar = CGraph::internal::CCHAR;
using CUint = CGraph::internal::CUINT;
using CSize = CGraph::internal::CSIZE;
using CVoid = CGraph::internal::CVOID;
using CVoidPtr = CGraph::internal::CVOID *;
using CInt = CGraph::internal::CINT;
using CLong = CGraph::internal::CLONG;
using CULong = CGraph::internal::CULONG;
using CBool = CGraph::internal::CBOOL;
using CIndex = CGraph::internal::CINT;                // 表示标识信息，可以为负数
using CFloat = CGraph::internal::CFLOAT;
using CDouble = CGraph::internal::CDOUBLE;
using CConStr = CGraph::internal::CCONSTR;             // 表示 const char*
using CBigBool = CGraph::internal::CBIGBOOL;

using CLevel = CGraph::internal::CINT;
using CSec = CGraph::internal::CLONG;                  // 表示秒信息, for second
using CMSec = CGraph::internal::CLONG;                 // 表示毫秒信息, for millisecond
using CFMSec = CGraph::internal::CDOUBLE;              // 表示毫秒信息，包含小数点信息

using CStatus = CGraph::internal::CSTATUS;
using CException = CGraph::internal::CEXCEPTION;

#endif //CGRAPH_CVALTYPE_H
