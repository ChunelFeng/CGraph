/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CObject.h
@Time: 2021/4/26 8:12 下午
@Desc: 
***************************/

#ifndef CGRAPH_COBJECT_H
#define CGRAPH_COBJECT_H

#include "CObjectDefine.h"

class CObject {
public:
    /**
     * 默认构造函数
     */
    explicit CObject() = default;

    /**
     * 初始化函数
     */
    virtual CSTATUS init() {
        return STATUS_OK;
    }

    /**
     * 流程处理函数，必须实现
     * @return
     */
    virtual CSTATUS run() = 0;

    /**
     * 逆初始化函数
     * @return
     */
    virtual CSTATUS deinit() {
        return STATUS_OK;
    }

    /**
     * 默认析构函数
     */
    virtual ~CObject() = default;
};


#endif //CGRAPH_COBJECT_H
