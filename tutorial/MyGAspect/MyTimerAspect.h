/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyTimerAspect.h
@Time: 2021/9/28 10:36 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYTIMERASPECT_H
#define CGRAPH_MYTIMERASPECT_H

#include <chrono>

#include "CGraph.h"

class MyTimerAspect : public CGraph::GAspect {
public:
    /**
     * 实现计时切面逻辑，记录 run() 方法的执行耗时信息
     */
    CStatus beginRun() override {
        start_ts_ = std::chrono::steady_clock::now();
        return CStatus();
    }

    CVoid finishRun(const CStatus& curStatus) override {
        std::chrono::duration<double, std::milli> span = std::chrono::steady_clock::now() - start_ts_;
        CGraph::CGRAPH_ECHO("----> [MyTimerAspect] [%s] time cost is : [%0.2lf] ms",
                            this->getName().c_str(), span.count());
    }

private:
    std::chrono::steady_clock::time_point start_ts_;
};

#endif //CGRAPH_MYTIMERASPECT_H
