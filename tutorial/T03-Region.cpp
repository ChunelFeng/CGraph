/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T03-Region.cpp
@Time: 2021/6/7 8:58 下午
@Desc: 本例主要演示，GRegion的使用方法，插入的元素按依赖关系执行
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_region() {
    CStatus status;
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b_region, c = nullptr;

    GElementPtr b1, b2, b3, b4 = nullptr;
    b1 = pipeline->createGNode<MyNode1>(GNodeInfo({}, "nodeB1", 1));    // 创建名为nodeB1的node信息
    b2 = pipeline->createGNode<MyNode2>(GNodeInfo({b1}, "nodeB2", 2));    // 创建名为nodeB2且自循环2次的node信息
    b3 = pipeline->createGNode<MyNode1>(GNodeInfo({b1}, "nodeB3", 1));
    b4 = pipeline->createGNode<MyNode1>(GNodeInfo({b2, b3}, "nodeB4", 1));

    b_region = pipeline->createGGroup<GRegion>({b1, b2, b3, b4});    // 将 b1、b2、b3、b4 注册入b_region中
    if (nullptr == b_region) {
        return;
    }

    status += pipeline->registerGElement<MyNode1>(&a, {}, "nodeA", 1);
    status += pipeline->registerGElement<GRegion>(&b_region, {a}, "regionB", 2);    // 将名为regionB，依赖a执行且自循环2次的region信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&c, {b_region}, "nodeC", 1);
    if (!status.isOK()) {
        return;
    }

    status = pipeline->process();
    CGRAPH_ECHO("pipeline process status is : [%d]", status.getCode());

    GPipelineFactory::remove(pipeline);
}


int main () {
    tutorial_region();
    return 0;
}

