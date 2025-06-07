/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-performance-01.cpp
@Time: 2023/12/3 17:41
@Desc: 
***************************/

#include "../_Materials/TestInclude.h"

using namespace CGraph;

void test_performance_01() {
    // 并行的执行32次，对应第1个例子，8thread，32并发，50w次
    GPipelinePtr pipeline = GPipelineFactory::create();
    CStatus status;
    const int runTimes = 500000;
    const int size = 32;
    GElementPtr arr[size] = {};

    UThreadPoolConfig config;
    config.default_thread_size_ = 8;
    config.secondary_thread_size_ = 0;
    config.max_task_steal_range_ = 7;
    config.max_thread_size_ = 8;
    config.primary_thread_empty_interval_ = 0;
    config.primary_thread_busy_epoch_ = 500;
    pipeline->setUniqueThreadPoolConfig(config);
    for (auto& i : arr) {
        pipeline->registerGElement<TestAdd1GNode>(&i);
    }
    status += pipeline->init();

    {
        UTimeCounter counter("test_performance_01");
        for (int t = 0; t < runTimes; t++) {
            pipeline->run();
        }
    }

    if ((runTimes * size) != g_test_node_cnt) {
        std::cout << "test_performance_01: g_test_node_cnt is not right : " << g_test_node_cnt << std::endl;
    }

    status += pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


int main() {
    test_performance_01();
    return 0;
}
