/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-performance-03.cpp
@Time: 2023/12/3 18:29
@Desc: 
***************************/

#include "../_Materials/TestInclude.h"

using namespace CGraph;

void test_performance_03() {
    // 简单dag场景，对应第三个例子，2thread，dag，100w次
    GPipelinePtr pipeline = GPipelineFactory::create();
    CStatus status;
    GElementPtr a,b1,b2,c1,c2,d = nullptr;
    const int runTimes = 1000000;

    UThreadPoolConfig config;
    config.default_thread_size_ = 2;
    config.secondary_thread_size_ = 0;
    config.max_task_steal_range_ = 1;
    config.max_thread_size_ = 2;
    config.primary_thread_empty_interval_ = 0;
    config.primary_thread_busy_epoch_ = 500;
    config.monitor_enable_ = false;    // 关闭扩缩容机制
    pipeline->setUniqueThreadPoolConfig(config);
    pipeline->registerGElement<TestAdd1GNode>(&a);
    pipeline->registerGElement<TestAdd1GNode>(&b1, {a});
    pipeline->registerGElement<TestAdd1GNode>(&b2, {b1});
    pipeline->registerGElement<TestAdd1GNode>(&c1, {a});
    pipeline->registerGElement<TestAdd1GNode>(&c2, {c1});
    pipeline->registerGElement<TestAdd1GNode>(&d, {b2, c2});
    status += pipeline->init();

    {
        UTimeCounter counter("test_performance_03");
        for (int t = 0; t < runTimes; t++) {
            pipeline->run();
        }
    }

    if ((runTimes * 6) != g_test_node_cnt) {
        std::cout << "test_performance_03: g_test_node_cnt is not right : " << g_test_node_cnt << std::endl;
    }

    status += pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


int main() {
    test_performance_03();
    return 0;
}
