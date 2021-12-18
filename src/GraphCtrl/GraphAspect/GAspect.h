/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspect.h
@Time: 2021/10/2 9:46 下午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECT_H
#define CGRAPH_GASPECT_H

#include <vector>

#include "GAspectObject.h"

CGRAPH_NAMESPACE_BEGIN

class GAspect : public GAspectObject {
public:
    /**
     * init()函数切面开始期间动作
     */
    virtual CStatus beginInit() {
        return CStatus();
    }

    /**
     * init()函数切面结束期间动作
     */
    virtual void finishInit(CStatus curStatus) {}

    /**
     * run()函数切面开始期间动作
     */
    virtual CStatus beginRun() {
        return CStatus();
    }

    /**
     * run()函数切面结束期间动作
     */
    virtual void finishRun(CStatus curStatus) {}

    /**
     * deinit()函数切面开始期间动作
     */
    virtual CStatus beginDeinit() {
        return CStatus();
    }

    /**
     * deinit()函数切面结束期间动作
     */
    virtual void finishDeinit(CStatus curStatus) {}
};

using GAspectPtr = GAspect *;
using GAspectPtrArr = std::vector<GAspectPtr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GASPECT_H
