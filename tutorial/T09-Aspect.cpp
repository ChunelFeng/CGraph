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
#include "MyGAspect/MyTemplateAspect.h"

using namespace CGraph;

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
    a->addGAspect<MyTraceAspect>();

    /** 针对node类型，添加可变参的MyTemplateAspect 切面逻辑 */
    a->addGAspect<MyTemplateAspect<int, double>>(20, 7.0);

    /** 针对group类型，添加 MyTimerAspect 切面逻辑 */
    b_region->addGAspect<MyTimerAspect>();

    /**
     * 给特定的element，统一添加 MyTraceAspect 类型的切面
     * 不传参数，表示对pipeline内部所有的节点，添加该切面
     * */
    pipeline->addGAspect<MyTraceAspect>({b_region, c});

    pipeline->process();    // 运行pipeline
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_aspect();
    return 0;
}
