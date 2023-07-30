/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPerfDefine.h
@Time: 2023/7/29 22:38
@Desc: 
***************************/

#ifndef CGRAPH_GPERFDEFINE_H
#define CGRAPH_GPERFDEFINE_H

#include <vector>

#include "../../GraphDefine.h"

CGRAPH_NAMESPACE_BEGIN

struct GPerfInfo : public CStruct {
    CUint loop_ = 0;                    // 循环数
    CUint first_start_ts_ = 0;          // 开始的时间戳(累计值)
    CUint accu_cost_ts_ = 0;            // 总体的耗时信息(累计值)
};

using GPerfInfoPtr = GPerfInfo *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPERFDEFINE_H
