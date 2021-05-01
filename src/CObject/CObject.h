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
     * 构造函数
     */
    CObject();

    /**
     * 初始化函数
     */
    virtual CSTATUS init();

    /**
     * 流程处理函数，必须实现
     * @return
     */
    virtual CSTATUS run() = 0;

    /**
     * 逆初始化函数
     * @return
     */
    virtual CSTATUS deinit();

    /**
     * 析构函数
     */
    virtual ~CObject();
};


#endif //CGRAPH_COBJECT_H
