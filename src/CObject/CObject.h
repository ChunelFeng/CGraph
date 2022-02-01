/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CObject.h
@Time: 2021/4/26 8:12 下午
@Desc: 所有类型的父节点，其中run()方法必须实现
***************************/

#ifndef CGRAPH_COBJECT_H
#define CGRAPH_COBJECT_H

#include "CObjectDefine.h"
#include "CStatus.h"

CGRAPH_NAMESPACE_BEGIN

class CObject {
public:
    /**
     * 默认构造函数
     */
    explicit CObject() = default;

    /**
     * 初始化函数
     */
    virtual CStatus init() {
        CGRAPH_COMMON_FUNCTION
    }

    /**
     * 流程处理函数
     */
    virtual CStatus run() = 0;

    /**
     * 释放函数（对应原先deinit函数）
     */
    virtual CStatus destroy() {
        CGRAPH_COMMON_FUNCTION
    }

    /**
     * 默认析构函数
     */
    virtual ~CObject() = default;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_COBJECT_H
