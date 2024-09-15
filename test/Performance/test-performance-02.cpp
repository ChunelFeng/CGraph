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
    // 串行执行32次，对应第二个例子，1thread，32串行，100w次
    GPipelinePtr pipeline = GPipelineFactory::create();
    CStatus status;
    const int runTimes = 1000000;
    const int size = 32;
    GElementPtr arr[size];

    pipeline->registerGElement<TestAdd1GNode>(&arr[0]);
    for (int i = 1; i < size; i++) {
        pipeline->registerGElement<TestAdd1GNode>(&arr[i], {arr[i - 1]});
    }
    pipeline->makeSerial();
    status += pipeline->init();
    {
        UTimeCounter counter("test_performance_02");
        for (int t = 0; t < runTimes; t++) {
            pipeline->run();
        }
    }

    if ((runTimes * size) != g_test_node_cnt) {
        std::cout << "test_performance_02: g_test_node_cnt is not right : " << g_test_node_cnt << std::endl;
    }

    status += pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}

int main() {
    test_performance_02();
    return 0;
}
