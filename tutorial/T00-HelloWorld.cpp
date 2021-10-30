/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T00-HelloWorld.cpp
@Time: 2021/6/24 1:03 上午
@Desc: 本例主要演示，GPipeline中注册GNode，并执行的流程
***************************/

#include "MyGNode/HelloWorldNode.h"

using namespace CGraph;

void tutorial_helloworld() {
    GPipelinePtr pipeline = GPipelineFactory::create();    // 创建一个pipeline，用于执行图框架信息
    GElementPtr hw = nullptr;

    pipeline->registerGElement<HelloWorldNode>(&hw);    // 注册一个helloworld节点
    pipeline->process();    // 运行pipeline
    GPipelineFactory::destroy(pipeline);
}


int main() {
    tutorial_helloworld();
    return 0;
}
