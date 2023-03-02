/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEventDefine.h
@Time: 2023/3/2 00:19
@Desc: 
***************************/

#ifndef CGRAPH_GEVENTDEFINE_H
#define CGRAPH_GEVENTDEFINE_H

#include "../GraphDefine.h"

CGRAPH_NAMESPACE_BEGIN

enum class G_EVENT_TYPE {
    SYNC = 0,        // 同步模式
    ASYNC = 1,       // 异步模式
};

CGRAPH_NAMESPACE_END

using GEventType = CGraph::G_EVENT_TYPE;    // 方便外部引用

#endif //CGRAPH_GEVENTDEFINE_H
