/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: URandomDefine.h
@Time: 2022/5/14 11:46
@Desc: 
***************************/

#ifndef CGRAPH_URANDOMDEFINE_H
#define CGRAPH_URANDOMDEFINE_H

#include <random>

#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN

using CGRAPH_RANDOM_MT19937 = std::mt19937;                // 梅森旋转法
using CGRAPH_RANDOM_MINSTD_RAND = std::minstd_rand;        // 线性同余法
using CGRAPH_RANDOM_RANLUX24_BASE = std::ranlux24_base;    // 滞后Fibonacci

const static CInt CGRAPH_REAL_RANDOM = 0;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_URANDOMDEFINE_H
