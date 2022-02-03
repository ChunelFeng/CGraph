/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T13-Daemon.cpp
@Time: 2022/2/3 11:31 上午
@Desc: 本例子主要展示，添加Daemon节点完成对pipeline的实时
***************************/

#include "MyGDaemon/MyDaemon.h"
#include "MyGNode/MyNode1.h"

using namespace CGraph;

void tutorial_daemon() {
    GPipelinePtr pipeline = GPipelineFactory::create();    // 创建一个pipeline，用于执行图框架信息
    GElementPtr a, b, c, d = nullptr;
    CStatus status;

    status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA", 10);
    status = pipeline->registerGElement<MyNode1>(&b, {}, "nodeB", 10);
    status = pipeline->registerGElement<MyNode1>(&c, {}, "nodeC", 10);
    status = pipeline->registerGElement<MyNode1>(&d, {}, "nodeD", 10);
    pipeline->addGDaemon<MyDaemon>();

    pipeline->process();
    GPipelineFactory::remove(pipeline);
}

int main() {
    tutorial_daemon();
    return 0;
}