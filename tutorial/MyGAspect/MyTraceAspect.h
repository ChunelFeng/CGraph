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

class MyTraceAspect : public CGraph::GAspect {
public:
    /**
     * 实现几个切面函数逻辑，模拟在对应执行的位置，打印trace信息
     */
    CStatus beginInit() override {
        CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] init begin ...", this->getName().c_str());
        return CStatus();
    }

    void finishInit(CStatus curStatus) override {
        CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] init finished, error code is [%d] ...", this->getName().c_str(),
                            curStatus.getCode());
    }

    CStatus beginRun() override {
        CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] run begin ...", this->getName().c_str());
        return CStatus();
    }

    void finishRun(CStatus curStatus) override {
        if (!curStatus.isEnable()) {
            CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] run finished, status is ok ...", this->getName().c_str());
        } else {
            CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] run finished, error code is [%d] ...", this->getName().c_str(),
                                curStatus.getCode());
        }
    }

    CStatus beginDeinit() override {
        CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] deinit begin ...", this->getName().c_str());
        return CStatus();
    }

    void finishDeinit(CStatus curStatus) override {
        CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] deinit finished, error code is [%d] ...", this->getName().c_str(),
                            curStatus.getCode());
    }
};

#endif //CGRAPH_MYTRACEASPECT_H
