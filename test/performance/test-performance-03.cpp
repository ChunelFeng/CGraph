/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-performance-03.cpp
@Time: 2023/12/3 18:29
@Desc: 
***************************/

#include "../_materials/TestGNodes.h"

using namespace CGraph;

void test_performance_03() {
    // 简单dag场景，对应第三个例子，2thread，dag，100w次
    GPipelinePtr pipeline = GPipelineFactory::create();
    CStatus status;
    GElementPtr a,b1,b2,c1,c2,d = nullptr;
    UThreadPoolConfig config;
    config.default_thread_size_ = 2;    // 我的笔记本，是8核心的 macbook pro m1
    config.secondary_thread_size_ = 0;
    config.max_task_steal_range_ = 1;
    config.max_thread_size_ = 2;
    config.primary_thread_empty_interval_ = 1;
    config.primary_thread_busy_epoch_ = 500;
    config.monitor_enable_ = false;    // 关闭扩缩容机制
    config.primary_thread_policy_ = CGRAPH_THREAD_SCHED_RR;
    config.primary_thread_priority_ = 10;
    pipeline->setUniqueThreadPoolConfig(config);
    pipeline->setAutoCheck(false);
    pipeline->registerGElement<TestMaterialAdd1GNode>(&a);
    pipeline->registerGElement<TestMaterialAdd1GNode>(&b1, {a});
    pipeline->registerGElement<TestMaterialAdd1GNode>(&b2, {b1});
    pipeline->registerGElement<TestMaterialAdd1GNode>(&c1, {a});
    pipeline->registerGElement<TestMaterialAdd1GNode>(&c2, {c1});
    pipeline->registerGElement<TestMaterialAdd1GNode>(&d, {b2, c2});
    status += pipeline->init();

    {
        UTimeCounter counter;
        for (int t = 0; t < 1000000; t++) {
            pipeline->run();
        }
    }

    status += pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}

int main() {
    test_performance_03();
    return 0;
}
