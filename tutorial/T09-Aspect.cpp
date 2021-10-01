/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T09-Aspect.cpp
@Time: 2021/9/28 10:40 下午
@Desc:
***************************/

#include "MyGNode/HelloWorldNode.h"
#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGAspect/MyTimerAspect.h"
#include "MyGAspect/MyLoggerAspect.h"


void tutorial_aspect() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c = nullptr;
    GElementPtr d_cluster = pipeline->createGGroup<GCluster>({
        /* 在Cluster中的节点，添加切面逻辑 */
        pipeline->createGNode<MyTimerAspect<MyNode2>>(GNodeInfo("nodeD1", 1)),
        pipeline->createGNode<MyNode1>(GNodeInfo("nodeD2", 1))
    });

    /* 给 HelloWorldNode 类型的节点，添加 MyLoggerAspect 类型的切面 */
    pipeline->registerGElement<MyLoggerAspect<HelloWorldNode>>(&a, {}, "nodeA");
    pipeline->registerGElement<MyTimerAspect<MyNode1>>(&b, {a}, "nodeB", 2);
    /* 给 MyNode1 类型的节点，依次添加 MyTimerAspect 和 MyLoggerAspect 类型的切面 */
    pipeline->registerGElement<MyLoggerAspect<MyTimerAspect<MyNode1>>>(&c, {b}, "nodeC", 1);

    pipeline->registerGElement<GGroup>(&d_cluster, {c}, "clusterD");
    pipeline->process();    // 运行pipeline
    GPipelineFactory::destroy(pipeline);
}


int main() {
    tutorial_aspect();
    return 0;
}
