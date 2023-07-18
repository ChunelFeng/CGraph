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
#include "CStatus.h"
#include "CFuncType.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * 所有框架内部结构体定义的基类
 * 仅针对类似 bean 数据类型的定义
 */
class CStruct {
    /**
     * 初始化所有字段的值信息
     * @return
     */
     virtual CStatus setup() {
         CGRAPH_EMPTY_FUNCTION
     }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CSTRUCT_H
