/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UTimeCounter.h
@Time: 2023/12/3 17:49
@Desc: 
***************************/

#ifndef CGRAPH_UTIMECOUNTER_H
#define CGRAPH_UTIMECOUNTER_H

#include <string>
#include <chrono>

#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN

class UTimeCounter : public UtilsObject {
public:
    explicit UTimeCounter() {
        key_ = CGRAPH_DEFAULT;
        start_ts_ = std::chrono::steady_clock::now();
    }

    explicit UTimeCounter(const std::string& key) {
        start_ts_ = std::chrono::steady_clock::now();
        key_ = key;
    }

    ~UTimeCounter() override {
        std::chrono::duration<double, std::milli> span = std::chrono::steady_clock::now() - start_ts_;
        CGraph::CGRAPH_ECHO("[%s]: time counter is : [%0.2lf] ms", key_.c_str(), span.count());
    }

private:
    std::chrono::steady_clock::time_point start_ts_;
    std::string key_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTIMECOUNTER_H
