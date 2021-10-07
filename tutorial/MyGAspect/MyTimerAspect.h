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

#include "../../src/CGraph.h"

class MyTimerAspect : public GAspect {
public:
    /**
     * 实现计时切面逻辑，记录 run() 方法的执行耗时信息
     */
    CSTATUS beginRun() override {
        start_ts_ = std::chrono::high_resolution_clock::now();
        return STATUS_OK;
    }

    void finishRun(CSTATUS curStatus) override {
        std::chrono::duration<double, std::milli> span = std::chrono::high_resolution_clock::now() - start_ts_;
        CGRAPH_ECHO("----> [MyTimerAspect] [%s] time cost is : [%0.2lf] ms", this->getName().c_str(), span.count());
    }

private:
    std::chrono::high_resolution_clock::time_point start_ts_;
};

#endif //CGRAPH_MYTIMERASPECT_H
