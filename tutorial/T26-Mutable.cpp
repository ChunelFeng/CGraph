/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T26-Mutable.cpp
@Time: 2023/11/15 21:18
@Desc: 
***************************/

#include "MyGMutable/MyMutable.h"
#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGNode/MyWriteParamNode.h"

using namespace CGraph;

void tutorial_mutable() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b_mutable, c, d = nullptr;

    // 创建一个 mutable（异变），在其中注入三个node
    // ps：这里设定的依赖信息关系，是无效的
    b_mutable = pipeline->createGGroup<MyMutable>({
        pipeline->createGNode<MyNode1>(GNodeInfo("nodeB1")),
        pipeline->createGNode<MyNode2>(GNodeInfo("nodeB2")),
        pipeline->createGNode<MyNode1>(GNodeInfo("nodeB3"))
    });

    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA", 1);
    pipeline->registerGGroup(&b_mutable, {a}, "mutableB", 1);
    pipeline->registerGElement<MyNode2>(&c, {a}, "nodeC", 1);
    pipeline->registerGElement<MyWriteParamNode>(&d, {b_mutable, c}, "nodeD", 1);

    // 执行多次，查看 b_mutable 中的执行结果。每次都是不同的
    pipeline->process(6);
    GPipelineFactory::remove(pipeline);
}


int main () {
    tutorial_mutable();
    return 0;
}
