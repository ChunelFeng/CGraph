/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyTraceAspect.h
@Time: 2021/9/29 10:51 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYTRACEASPECT_H
#define CGRAPH_MYTRACEASPECT_H

#include "../../src/CGraph.h"

class MyTraceAspect : public GAspect {
public:
    /**
     * 实现几个切面函数逻辑，模拟在对应执行的位置，打印trace信息
     */
    CSTATUS beginInit() override {
        CGRAPH_ECHO("----> [MyTraceAspect] [%s] init begin ...", this->getName().c_str());
        return STATUS_OK;
    }

    void finishInit(CSTATUS curStatus) override {
        CGRAPH_ECHO("----> [MyTraceAspect] [%s] init finished, error code is [%d] ...", this->getName().c_str(), curStatus);
    }

    CSTATUS beginRun() override {
        CGRAPH_ECHO("----> [MyTraceAspect] [%s] run begin ...", this->getName().c_str());
        return STATUS_OK;
    }

    void finishRun(CSTATUS curStatus) override {
        if (STATUS_OK == curStatus) {
            CGRAPH_ECHO("----> [MyTraceAspect] [%s] run finished, status is ok ...", this->getName().c_str());
        } else {
            CGRAPH_ECHO("----> [MyTraceAspect] [%s] run finished, error code is [%d] ...", this->getName().c_str(), curStatus);
        }
    }

    CSTATUS beginDeinit() override {
        CGRAPH_ECHO("----> [MyTraceAspect] [%s] deinit begin ...", this->getName().c_str());
        return STATUS_OK;
    }

    void finishDeinit(CSTATUS curStatus) override {
        CGRAPH_ECHO("----> [MyTraceAspect] [%s] deinit finished, error code is [%d] ...", this->getName().c_str(), curStatus);
    }
};

#endif //CGRAPH_MYTRACEASPECT_H
