/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T07-MultiPipeline.cpp
@Time: 2021/8/27 8:44 下午
@Desc: 本例主要演示，多个pipeline同时执行的情况
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_pipeline_1(GPipelinePtr pipeline_1) {
    if (nullptr == pipeline_1) {
        return;
    }

    GElementPtr node1A, node1B, node1C = nullptr;

    pipeline_1->registerGElement<MyNode1>(&node1A, {}, "node1A");
    pipeline_1->registerGElement<MyNode1>(&node1B, {node1A}, "node1B");
    pipeline_1->registerGElement<MyNode1>(&node1C, {node1B}, "node1C");

    pipeline_1->process(5);    // 执行n次，本例中 n=5
}


void tutorial_pipeline_2(GPipelinePtr pipeline_2) {
    if (nullptr == pipeline_2) {
        return;
    }

    GElementPtr node2A, node2B, node2C = nullptr;

    pipeline_2->registerGElement<MyNode2>(&node2A, {}, "node2A");
    pipeline_2->registerGElement<MyNode2>(&node2B, {node2A}, "node2B");
    pipeline_2->registerGElement<MyNode2>(&node2C, {node2A}, "node2C");

    pipeline_2->process(3);
}


void tutorial_pipeline_3(GPipelinePtr pipeline_3) {
    if (nullptr == pipeline_3) {
        return;
    }

    CStatus status;
    GElementPtr node3A, node3B, node3C, node3D = nullptr;
    GElementPtr region = nullptr;

    node3A = pipeline_3->createGNode<MyNode1>(GNodeInfo({}, "node3A", 1));
    node3B = pipeline_3->createGNode<MyNode2>(GNodeInfo({node3A}, "node3B", 1));
    node3C = pipeline_3->createGNode<MyNode1>(GNodeInfo({node3A}, "node3C", 1));
    node3D = pipeline_3->createGNode<MyNode1>(GNodeInfo({node3B, node3C}, "node3D", 1));

    region = pipeline_3->createGGroup<GRegion>({node3A, node3B, node3C, node3D});

    pipeline_3->registerGElement<GRegion>(&region);
    pipeline_3->process(2);
}


void tutorial_multi_pipeline() {
    GPipelinePtr pipeline_1 = GPipelineFactory::create();
    GPipelinePtr pipeline_2 = GPipelineFactory::create();
    GPipelinePtr pipeline_3 = GPipelineFactory::create();

    /**
     * 设置个别pipeline的内部 thread pool 资源信息，用以减少整体资源占用（可选）
     * 这里主要是为了说明，多个pipeline一起运行的时候，可以通过接口，针对个别pipeline进行调度资源的设置
     */
    UThreadPoolConfig config;
    config.default_thread_size_ = 4;
    config.max_thread_size_ = 4;
    config.monitor_enable_ = false;
    UThreadPool pool(true, config);    // 开辟一个4个线程的线程池，直接 init，并且参数设置为 config

    /**
     * 本例中，pipeline1 和 pipeline2 的并发逻辑相对简单
     * 通过如下接口，将这两个pipeline中的调度资源，修改为同一个线程池。
     * ps：注意，必须在 pipeline init之前，先init传入的线程池
     */
    pipeline_1->setSharedThreadPool(&pool);
    pipeline_2->setSharedThreadPool(&pool);

    /**
     * 经过上述的设置，pipeline1 和 pipeline2 共享同一个线程池，去调度其中的dag逻辑
     * pipeline3 没有设定，故使用自带的默认线程池完成自己的调度逻辑
     */
    std::thread thd1 = std::move(std::thread(tutorial_pipeline_1, pipeline_1));
    std::thread thd2 = std::move(std::thread(tutorial_pipeline_2, pipeline_2));
    std::thread thd3 = std::move(std::thread(tutorial_pipeline_3, pipeline_3));

    thd1.join();
    thd2.join();
    thd3.join();

    GPipelineFactory::clear();
}


int main() {
    tutorial_multi_pipeline();
    return 0;
}

