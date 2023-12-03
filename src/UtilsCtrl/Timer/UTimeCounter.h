/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UTimeCounter.h
@Time: 2023/12/3 17:49
@Desc: 
***************************/

#ifndef CGRAPH_UTIMECOUNTER_H
#define CGRAPH_UTIMECOUNTER_H

#include <chrono>

#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN

class UTimeCounter : public UtilsObject {
public:
    explicit UTimeCounter() {
        start_ts_ = std::chrono::steady_clock::now();
    }

    ~UTimeCounter() override {
        std::chrono::duration<double, std::milli> span = std::chrono::steady_clock::now() - start_ts_;
        CGraph::CGRAPH_ECHO("time counter is : [%0.2lf] ms", span.count());
    }

private:
    std::chrono::steady_clock::time_point start_ts_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTIMECOUNTER_H
