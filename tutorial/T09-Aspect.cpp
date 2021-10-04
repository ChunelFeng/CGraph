/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T09-Aspect.cpp
@Time: 2021/9/28 10:40 下午
@Desc: 本例主要演示，在element中添加切面的逻辑
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGAspect/MyTimerAspect.h"
#include "MyGAspect/MyTraceAspect.h"


void tutorial_aspect() {
    GPipelinePtr pipeline = GPipelineFactory::create();

    GElementPtr a, b_region, c = nullptr;
    b_region = pipeline->createGGroup<GRegion>({
        pipeline->createGNode<MyNode1>(GNodeInfo({},"nodeB1", 1)),
        pipeline->createGNode<MyNode2>(GNodeInfo({}, "nodeB2", 2))
    });

    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA", 1);
    pipeline->registerGElement<GRegion>(&b_region, {a}, "regionB", 1);
    pipeline->registerGElement<MyNode1>(&c, {b_region}, "nodeC", 1);

    /** 针对node类型，添加 MyTraceAspect 切面逻辑 */
    a->addAspect<MyTraceAspect>();

    /** 针对group类型，添加 MyTimerAspect 切面逻辑 */
    b_region->addAspect<MyTimerAspect>();

    /** 可以对同一个节点，设置多个切面(可重复添加)。此时，切面会执行多次 */
    c->addAspect<MyTimerAspect>()->addAspect<MyTimerAspect>()->addAspect<MyTraceAspect>();

    pipeline->process();    // 运行pipeline
    GPipelineFactory::destroy(pipeline);
}


int main() {
    tutorial_aspect();
    return 0;
}
