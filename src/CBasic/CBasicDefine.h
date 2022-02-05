/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CBasicDefine.h
@Time: 2021/4/26 8:15 下午
@Desc:
***************************/

#ifndef CGRAPH_CBASICDEFINE_H
#define CGRAPH_CBASICDEFINE_H

#define CGRAPH_NAMESPACE_BEGIN                                          \
namespace CGraph {                                                      \

#define CGRAPH_NAMESPACE_END                                            \
} /* end of namespace CGraph */                                         \

CGRAPH_NAMESPACE_BEGIN

using CUINT = unsigned int;
using CVOID = void;
using CINT = int;
using CLONG = long;
using CULONG = unsigned long;
using CBOOL = bool;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CBASICDEFINE_H
