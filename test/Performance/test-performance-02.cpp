/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-performance-02.cpp
@Time: 2023/12/3 18:28
@Desc: 
***************************/

#include "../_Materials/TestInclude.h"

using namespace CGraph;

void test_performance_02() {
    // 串行执行32次，对应第二个例子，1thread，32串行，1000w次
    GPipelinePtr pipeline = GPipelineFactory::create();
    CStatus status;
    GElementPtr arr[32];
    pipeline->registerGElement<TestAdd1GNode>(&arr[0]);
    for (int i = 1; i < 32; i++) {
        pipeline->registerGElement<TestAdd1GNode>(&arr[i], {arr[i - 1]});
    }
    pipeline->makeSerial();
    pipeline->setAutoCheck(false);
    status += pipeline->init();
    {
        UTimeCounter counter("test_performance_02");
        for (int t = 0; t < 1000000; t++) {
            pipeline->run();
        }
    }

    status += pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}

int main() {
    test_performance_02();
    return 0;
}
