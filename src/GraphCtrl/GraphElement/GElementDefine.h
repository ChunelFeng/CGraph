/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementDefine.h
@Time: 2022/4/9 15:31
@Desc: 
***************************/

#ifndef CGRAPH_GELEMENTDEFINE_H
#define CGRAPH_GELEMENTDEFINE_H

#include "../GraphObject.h"

CGRAPH_NAMESPACE_BEGIN

const static CMSec CGRAPH_DEFAULT_ELEMENT_RUN_TTL = 0;      // 线程超时时间设定，0为不设定超时信息，单位毫秒
const static CSize CGRAPH_DEFAULT_LOOP_TIMES = 1;           // 默认循环次数信息
const static CLevel CGRAPH_DEFAULT_ELEMENT_LEVEL = 0;       // 默认的element级别，用于控制init函数

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GELEMENTDEFINE_H
