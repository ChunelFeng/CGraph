/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T18-Event.cpp
@Time: 2023/1/20 17:55
@Desc: 本例子主要展示触发Event功能
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyWriteParamNode.h"
#include "MyGNode/MyEventNode.h"
#include "MyGEvent/MyPrintEvent.h"

using namespace CGraph;

void tutorial_event() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    CStatus status = pipeline->registerGElement<MyWriteParamNode>(&a, {}, "nodeA");
    status += pipeline->registerGElement<MyEventNode>(&b, {a}, "nodeB");
    status += pipeline->registerGElement<MyNode1>(&c, {b}, "nodeC");
    status += pipeline->registerGElement<MyEventNode>(&d, {b}, "nodeD");

    // 在pipeline中添加一个事件信息，通过 notify("my-print-event")触发
    pipeline->addGEvent<MyPrintEvent>("my-print-event");
    pipeline->process();

    GPipelineFactory::clear();
}


int main() {
    tutorial_event();
    return 0;
}
