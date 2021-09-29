/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspect.h
@Time: 2021/9/27 11:28 下午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECT_H
#define CGRAPH_GASPECT_H

#include "GAspectObject.h"


template <typename T>
class GAspect : public GAspectObject<T> {
public:
    /**
     * 切面开始期间动作
     */
    virtual void begin() = 0;


    /**
     * 切面结束期间动作
     */
    virtual void finish() = 0;


    /**
     * 切面执行函数
     * @return
     */
    CSTATUS run() override;
};

template <typename T>
using GAspectPtr = GAspect<T> *;

#include "GAspect.inl"

#endif //CGRAPH_GASPECT_H
