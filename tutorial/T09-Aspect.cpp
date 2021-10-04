/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T09-Aspect.cpp
@Time: 2021/9/28 10:40 下午
@Desc:
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGAspect/MyTimerAspect.h"
#include "MyGAspect/MyTraceAspect.h"


void tutorial_aspect() {
    GPipelinePtr pipeline = GPipelineFactory::create();

    GElementPtr a, b_region, c, d = nullptr;
    b_region = pipeline->createGGroup<GRegion>({
        pipeline->createGNode<MyNode1>(GNodeInfo({},"nodeB1", 1)),
        pipeline->createGNode<MyNode2>(GNodeInfo({}, "nodeB2", 2))
    });

    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA", 1);
    pipeline->registerGElement<GRegion>(&b_region, {a}, "regionB", 1);
    pipeline->registerGElement<MyNode1>(&c, {b_region}, "nodeC", 1);
    pipeline->registerGElement<MyNode2>(&d, {a}, "nodeD", 1);

    /** 针对节点a，添加MyTraceAspect切面逻辑 */
    a->addAspect<MyTraceAspect>();

    /** 针对group信息，添加MyTimerAspect切面逻辑 */
    b_region->addAspect<MyTimerAspect>();

    /** 可以一次性添加1个/2个切面。添加3个或3个以上切面，请多次调用addAspect()方法(操作见下) */
    c->addAspect<MyTraceAspect, MyTimerAspect>();

    /** 可以对同一个节点，设置多个切面(可重复添加)。此时，切面会执行多次 */
    d->addAspect<MyTimerAspect>()->addAspect<MyTimerAspect>()->addAspect<MyTraceAspect>();

    pipeline->process();    // 运行pipeline
    GPipelineFactory::destroy(pipeline);
}


int main() {
    tutorial_aspect();
    return 0;
}
