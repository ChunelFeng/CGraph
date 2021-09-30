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

    /* 给 HelloWorldNode 类型的节点，添加 MyLoggerAspect 类型的切面 */
    pipeline->registerGElement<MyLoggerAspect<HelloWorldNode>>(&a, {}, "nodeA");

    pipeline->registerGElement<MyTimerAspect<MyNode1>>(&b, {a}, "nodeB", 2);

    /* 给 MyNode2 类型的节点，依次添加 MyLoggerAspect 和 MyTimerAspect 类型的切面 */
    pipeline->registerGElement<MyTimerAspect<MyLoggerAspect<MyNode2>>>(&c, {b}, "nodeC");
    pipeline->process();    // 运行pipeline
    GPipelineFactory::destroy(pipeline);
}


int main() {
    tutorial_aspect();
    return 0;
}