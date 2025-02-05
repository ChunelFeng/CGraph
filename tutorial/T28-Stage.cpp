/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T28-Stage.cpp
@Time: 2024/12/15 18:40
@Desc: 本例主要展示在多个并发的element中，通过 GStage 做同步的功能
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyStageNode.h"

using namespace CGraph;

void tutorial_stage() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d, e = nullptr;

    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    pipeline->registerGElement<MyStageNode<1, 2>>(&b, {a}, "nodeB");
    pipeline->registerGElement<MyStageNode<3, 1>>(&c, {a}, "nodeC");
    pipeline->registerGElement<MyStageNode<3, 1>>(&d, {a}, "nodeD");
    pipeline->registerGElement<MyNode1>(&e, {b, c, d}, "nodeE");

    /**
     * 添加一个 stage 信息，遇到 3次的时候，会通知触发结束
     * 正好对应上面三个 MyStageNode 的逻辑
     */
    pipeline->addGStage<GStage>(kStageKey, 3);

    pipeline->process();
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_stage();
    return 0;
}
