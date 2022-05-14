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

using CGRAPH_RANDOM_MT19937 = std::mt19937;        // 设定随机方式

const static CInt CGRAPH_REAL_RANDOM = 0;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_URANDOMDEFINE_H
