/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CStruct.h
@Time: 2023/7/16 11:36
@Desc: 
***************************/

#ifndef CGRAPH_CSTRUCT_H
#define CGRAPH_CSTRUCT_H

#include "CBasicDefine.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * 所有框架内部结构体定义的基类
 * 仅针对类似 pod 数据类型的定义
 */
struct CStruct {
    virtual ~CStruct() = default;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CSTRUCT_H
