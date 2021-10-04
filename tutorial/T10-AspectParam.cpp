/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T10-AspectParam.cpp
@Time: 2021/10/4 11:23 下午
@Desc: 本例主要演示，在切面流程中加入自定义参数的逻辑
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGAspect/MyConnAspect.h"
#include "MyGParam/MyConnParam.h"
#include "MyGAspect/MyTimerAspect.h"

void tutorial_aspect_param() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    MyConnParam paramA;
    paramA.ip = "127.0.0.1";
    paramA.port = 6666;

    MyConnParam paramB;
    paramB.ip = "255.255.255.255";
    paramB.port = 9999;

    GElementPtr a, b = nullptr;
    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA", 1);
    pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB", 1);

    /** 给a节点添加 MyConnAspect 切面的逻辑，并且传入 paramA 相关参数 */
    a->addAspect<MyConnAspect, MyConnParam>(&paramA);

    /** 给b节点添加多个切面，有的传递参数，有的不传递 */
    b->addAspect<MyConnAspect, MyConnParam>(&paramB)->addAspect<MyTimerAspect>();

    pipeline->process();
    GPipelineFactory::clear();
}

int main() {
    tutorial_aspect_param();
    return 0;
}
