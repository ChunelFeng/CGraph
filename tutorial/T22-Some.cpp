/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T22-Some.cpp
@Time: 2023/8/20 13:39
@Desc: 本例主要演示，GSome的使用方法，插入的元素并发执行n个完毕后，整体结束（本例为 beta版本）
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyAsyncNode.h"

using namespace CGraph;

void tutorial_some() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b_some, c, d = nullptr;

    /**
     * 创建一个类型为GSome的组，其中注入3个异步节点，其中有[1]个执行完成，则GSome执行结束
     * 需要注意的是，当前GSome执行完成，并且今后后续的element后，
     * 内部的异步节点仍会继续执行，故如果在异步节点内，做参数处理，请注意考虑后续影响
     * 当前pipeline的run()方法，会等到内部的异步节点全部执行完成后，才结束
     */
    b_some = pipeline->createGGroup<GSome<1>>({
        /**
         * 注入三个异步节点，分别sleep 5/2/8s,
         * 执行完 sleep(2s)的异步节点后，当前的 GSome结束。内部其他节点继续执行
         * 注意：异步节点只允许 loop=1，否则运行时会报错
         */
        pipeline->createGNode<MyAsyncNode<5>>(GNodeInfo("asyncNodeB1")),
        pipeline->createGNode<MyAsyncNode<2>>(GNodeInfo("asyncNodeB2")),
        pipeline->createGNode<MyAsyncNode<8>>(GNodeInfo("asyncNodeB3"))
    });

    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    status += pipeline->registerGElement<GSome<1>>(&b_some, {a}, "someB");
    status += pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    status += pipeline->registerGElement<MyNode1>(&d, {b_some, c}, "nodeD");
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
