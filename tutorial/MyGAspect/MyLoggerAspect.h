/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyLogAspect.h
@Time: 2021/9/29 10:51 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYLOGGERASPECT_H
#define CGRAPH_MYLOGGERASPECT_H

#include "../../src/CGraph.h"

template <typename T>
class MyLoggerAspect : public GAspect<T> {
public:
    /**
     * 实现几个切面函数逻辑，模拟在对应执行的位置，输出log内容
     */
    void beginInit() override {
        CGRAPH_ECHO("[MyLoggerAspect] ----> [%s] begin init ...", this->getName().c_str());
    }

    /**
     * 根据run方法执行的结果，记录对应的信息
     * @param status
     */
    void finishRun(CSTATUS status) override {
        if (STATUS_OK == status) {
            CGRAPH_ECHO("[MyLoggerAspect] ----> [%s] run finished, status is ok ...", this->getName().c_str());
        } else {
            CGRAPH_ECHO("[MyLoggerAspect] ----> [%s] run finished, error code is [%d] ...", this->getName().c_str(), status);
        }
    }

    void beginDeinit() override {
        CGRAPH_ECHO("[MyLoggerAspect] ----> [%s] begin deinit ...", this->getName().c_str());
    }
};

#endif //CGRAPH_MYLOGGERASPECT_H
