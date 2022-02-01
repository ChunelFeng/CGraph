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
        CGRAPH_COMMON_FUNCTION
    }

    /**
     * init()函数切面结束期间动作
     */
    virtual void finishInit(CStatus curStatus) {}

    /**
     * run()函数切面开始期间动作
     */
    virtual CStatus beginRun() {
        CGRAPH_COMMON_FUNCTION
    }

    /**
     * run()函数切面结束期间动作
     */
    virtual void finishRun(CStatus curStatus) {}

    /**
     * destroy()函数切面开始期间动作
     */
    virtual CStatus beginDestroy() {
        CGRAPH_COMMON_FUNCTION
    }

    /**
     * destroy()函数切面结束期间动作
     */
    virtual void finishDestroy(CStatus curStatus) {}
};

using GAspectPtr = GAspect *;
using GAspectPtrArr = std::vector<GAspectPtr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GASPECT_H
