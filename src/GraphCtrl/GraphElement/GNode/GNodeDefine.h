/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNodeDefine.h
@Time: 2022/1/24 9:56 下午
@Desc: 
***************************/

#ifndef CGRAPH_GNODEDEFINE_H
#define CGRAPH_GNODEDEFINE_H

#include "../GElement.h"

CGRAPH_NAMESPACE_BEGIN

enum class GNodeType {
    BASIC = 0,
    IO = 1,
    CPU = 2,
    GPU = 3
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GNODEDEFINE_H