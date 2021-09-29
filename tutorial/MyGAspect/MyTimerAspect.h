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

template <typename T>
class MyTimerAspect : public GAspect<T> {
public:
    void begin() override {
        start_ts_ = std::chrono::high_resolution_clock::now();
    }

    void finish() override {
        std::chrono::duration<double, std::milli> time_span = std::chrono::high_resolution_clock::now() - start_ts_;
        CGRAPH_ECHO("----> this node time cost is : [%0.2lf] ms", time_span.count());
    }

private:
    std::chrono::high_resolution_clock::time_point start_ts_;
};

#endif //CGRAPH_MYTIMERASPECT_H
