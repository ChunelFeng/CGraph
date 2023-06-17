/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GConditionDefine.h
@Time: 2023/6/17 19:24
@Desc: 
***************************/


#ifndef CGRAPH_GCONDITIONDEFINE_H
#define CGRAPH_GCONDITIONDEFINE_H

#include "../GGroupDefine.h"

CGRAPH_NAMESPACE_BEGIN

enum class GMultiConditionType {
    SERIAL = 0,        // 串行执行
    PARALLEL = 1,      // 并行执行
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GCONDITIONDEFINE_H
