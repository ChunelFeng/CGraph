/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T00-HelloCGraph.cpp
@Time: 2021/6/24 1:03 上午
@Desc: 本例主要演示，GPipeline中注册GNode，并执行的流程
***************************/

#include "MyGNode/HelloCGraphNode.h"

using namespace CGraph;

void tutorial_hello_cgraph() {
    GPipelinePtr pipeline = GPipelineFactory::create();    // 创建一个pipeline，用于执行图框架信息
    GElementPtr hcg = nullptr;

    pipeline->registerGElement<HelloCGraphNode>(&hcg);    // 注册一个HelloCGraphNode节点，功能就是打印 "Hello, CGraph." 信息
    pipeline->process();    // 运行pipeline
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_hello_cgraph();
    return 0;
}
