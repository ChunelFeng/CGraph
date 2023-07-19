/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyTraceAspect.h
@Time: 2021/9/29 10:51 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYTRACEASPECT_H
#define CGRAPH_MYTRACEASPECT_H

#include "CGraph.h"

class MyTraceAspect : public CGraph::GAspect {
public:
    /**
     * 实现几个切面函数逻辑，模拟在对应执行的位置，打印trace信息
     */
    CStatus beginInit() override {
        CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] init begin ...", this->getName().c_str());
        return CStatus();
    }

    CVoid finishInit(const CStatus& curStatus) override {
        CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] init finished, error code is [%d] ...", this->getName().c_str(),
                            curStatus.getCode());
    }

    CStatus beginRun() override {
        CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] run begin ...", this->getName().c_str());
        return CStatus();
    }

    CVoid finishRun(const CStatus& curStatus) override {
        if (!curStatus.isOK()) {
            CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] run finished, status is ok ...", this->getName().c_str());
        } else {
            CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] run finished, error code is [%d] ...", this->getName().c_str(),
                                curStatus.getCode());
        }
    }

    CStatus beginDestroy() override {
        CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] destroy begin ...", this->getName().c_str());
        return CStatus();
    }

    CVoid finishDestroy(const CStatus& curStatus) override {
        CGraph::CGRAPH_ECHO("----> [MyTraceAspect] [%s] destroy finished, error code is [%d] ...", this->getName().c_str(),
                            curStatus.getCode());
    }
};

#endif //CGRAPH_MYTRACEASPECT_H
