/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CValTypes.h
@Time: 2022/2/3 12:58 下午
@Desc: 
***************************/

#ifndef CGRAPH_CVALTYPE_H
#define CGRAPH_CVALTYPE_H

#include "CStatus.h"
#include "CException.h"

using CChar = CGraph::CCHAR;
using CUint = CGraph::CUINT;
using CSec = CGraph::CUINT;            // 表示秒信息, for second
using CMSec = CGraph::CUINT;           // 表示毫秒信息, for millisecond
using CSize = CGraph::CSIZE;
using CVoid = CGraph::CVOID;
using CVoidPtr = CGraph::CVOID *;
using CInt = CGraph::CINT;
using CLevel = CGraph::CINT;
using CLong = CGraph::CLONG;
using CULong = CGraph::CULONG;
using CBool = CGraph::CBOOL;
using CIndex = CGraph::CINT;            // 表示标识信息，可以为负数
using CFloat = CGraph::CFLOAT;
using CDouble = CGraph::CDOUBLE;
using CConStr = CGraph::CCONSTR;    // 表示 const char*
using CBigBool = CGraph::CBIGBOOL;

using CStatus = CGraph::CSTATUS;
using CException = CGraph::CEXCEPTION;

#endif //CGRAPH_CVALTYPE_H
