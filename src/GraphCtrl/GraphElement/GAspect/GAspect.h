/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspect.h
@Time: 2021/9/27 11:28 下午
@Desc: 提供了6个位置的切面信息，供调用
***************************/

#ifndef CGRAPH_GASPECT_H
#define CGRAPH_GASPECT_H

#include "GAspectObject.h"


template <typename T>
class GAspect : public GAspectObject<T> {
public:
    /**
     * init()函数切面开始期间动作
     */
    virtual void beginInit();

    /**
     * init()函数切面结束期间动作
     */
    virtual void finishInit(CSTATUS status);

    /**
     * run()函数切面开始期间动作
     */
    virtual void beginRun();

    /**
     * run()函数切面结束期间动作
     */
    virtual void finishRun(CSTATUS status);

    /**
     * deinit()函数切面开始期间动作
     */
    virtual void beginDeinit();

    /**
     * init()函数切面结束期间动作
     */
    virtual void finishDeinit(CSTATUS status);
};

template <typename T>
using GAspectPtr = GAspect<T> *;

#include "GAspect.inl"

#endif //CGRAPH_GASPECT_H
