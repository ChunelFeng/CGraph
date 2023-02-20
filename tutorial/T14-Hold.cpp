/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T14-Hold.cpp
@Time: 2022/4/6 23:46
@Desc: 本例子主要展示，在MyHoldNode添加isHold()方法，来决定当前节点是否继续执行
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyHoldNode.h"

using namespace CGraph;

void tutorial_hold() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr holdNode, node1 = nullptr;

    pipeline->registerGElement<MyHoldNode>(&holdNode, {}, "myHold");    // 注册了一个实现了hold方法的节点
    pipeline->registerGElement<MyNode1>(&node1, {holdNode}, "node1");

    pipeline->process(3);    // 运行pipeline
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_hold();
    return 0;
}
