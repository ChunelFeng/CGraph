/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T25-Coordinator.cpp
@Time: 2023/9/8 22:01
@Desc: 本例主要演示，通过 GCoordinator 在pipeline执行的过程中，修改辅助线程数
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_coordinator() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d, f, g, h = nullptr;
    GCoordinatorPtr<-2> e_coordinator = nullptr;    // 申明一个减少2个(-2个)辅助线程的 GCoordinator

    UThreadPoolConfig config;
    config.default_thread_size_ = 0;
    config.secondary_thread_size_ = 4;    // 设置刚开始的时候，4个辅助线程执行
    pipeline->setUniqueThreadPoolConfig(config);

    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    status += pipeline->registerGElement<MyNode1>(&b, {}, "nodeB");
    status += pipeline->registerGElement<MyNode1>(&c, {}, "nodeC");
    status += pipeline->registerGElement<MyNode1>(&d, {}, "nodeD");
    /**
     * [a,b,c,d] 并发执行结束之后，加入 GCoordinator 逻辑，减少进程内的 2 个辅助线程
     * 使得后面的 f->[g,h] 的逻辑，执行的时候，减少线程切换逻辑
     * 从而在一定程度上，提升执行效率
     * 注意：推荐在非并发执行的位置使用 GCoordinator 功能。使用前，请预估好线程数
     */
    status += pipeline->registerGElement<GCoordinator<-2>>(&e_coordinator, {a, b, c, d}, "coordinatorE");
    status += pipeline->registerGElement<MyNode1>(&f, {e_coordinator}, "nodeF");
    status += pipeline->registerGElement<MyNode2>(&g, {f}, "nodeG");
    status += pipeline->registerGElement<MyNode2>(&h, {f}, "nodeH");

    pipeline->process();
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_coordinator();
    return 0;
}
