/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T23-Some.cpp
@Time: 2023/8/20 13:39
@Desc: 本例主要演示，GSome的使用方法，插入的元素并发执行n个完毕后，整体结束
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_some() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b_some, c, d = nullptr;

    /**
     * 创建一个类型为GSome的组，其中注入3个异步节点，其中有[1]个执行完成，则GSome执行结束
     * 也就是说，这个 GSome 会执行1s后，就往下继续执行。效果类似增强版本的弱依赖
     * 需要注意的是，当前GSome执行完成，并且今后后续的element后，
     * 内部的节点仍会继续执行，故如果在节点内做参数处理，请注意考虑后续影响
     * 当前pipeline的run()方法，会等到内部的内部节点全部执行完成后，才结束
     */
    b_some = pipeline->createGGroup<GSome<1>>({
        pipeline->createGNode<MyNode1>(GNodeInfo("nodeB1")),
        pipeline->createGNode<MyNode2>(GNodeInfo("nodeB2")),
        pipeline->createGNode<MyNode2>(GNodeInfo("nodeB3"))
    });

    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    status += pipeline->registerGGroup(&b_some, {a}, "someB");
    status += pipeline->registerGElement<MyNode1>(&c, {b_some}, "nodeC");
    status += pipeline->registerGElement<MyNode2>(&d, {c}, "nodeD");
    if (!status.isOK()) {
        return;
    }

    status += pipeline->process();
    CGRAPH_ECHO("pipeline run finished, error code is [%d]", status.getCode());

    GPipelineFactory::clear();
}


int main() {
    tutorial_some();
    return 0;
}
