/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-performance-04.cpp
@Time: 2024/8/11 14:11
@Desc: 
***************************/

#include "../_Materials/TestInclude.h"

using namespace CGraph;

void test_performance_04() {
    GPipelinePtr pipeline = GPipelineFactory::create();

    const int layer = 8;
    const int nodePerLayer = 8;
    const int runTimes = 100000;

    UThreadPoolConfig config;
    config.default_thread_size_ = nodePerLayer;
    config.max_thread_size_ = nodePerLayer;
    config.max_task_steal_range_ = nodePerLayer - 1;
    config.primary_thread_busy_epoch_ = 500;
    config.primary_thread_empty_interval_ = 0;
    pipeline->setUniqueThreadPoolConfig(config);

    // 实现一个全连接
    GElementPtrSet curLayer {};
    GElementPtrSet beforeLayer {};
    for (int i = 0; i < layer; i++) {
        for (int j = 0; j < nodePerLayer; j++) {
            GElementPtr ptr = nullptr;
            pipeline->registerGElement<TestAdd1GNode>(&ptr, beforeLayer);
            curLayer.insert(ptr);
        }

        beforeLayer = curLayer;
        curLayer.clear();
    }

    pipeline->init();
    {
        UTimeCounter counter("test_performance_04");
        for (int i = 0; i < runTimes; i++) {
            pipeline->run();
        }
    }
    pipeline->destroy();

    if ((layer * nodePerLayer * runTimes) != g_test_node_cnt) {
        std::cout << "test_performance_04: g_test_node_cnt is not right : " << g_test_node_cnt << std::endl;
    }

    GPipelineFactory::remove(pipeline);
}

int main() {
    test_performance_04();
    return 0;
}