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

enum class GElementType {
    ELEMENT = 0x00000000,                    // 元素
    NODE = 0x00010000,                       // 节点
    GROUP = 0x00020000,                      // 组
    CLUSTER = 0x00020001,                    // 簇
    REGION = 0x00020002,                     // 区域
    CONDITION = 0x00020004,                  // 条件
    ADAPTER = 0x00040000,                    // 适配器
    FUNCTION = 0x00040001,                   // 函数
    SINGLETON = 0x00040002,                  // 单例
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GELEMENTDEFINE_H
