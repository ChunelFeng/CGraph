/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T19-Cancel.cpp
@Time: 2023/5/21 21:21
@Desc: 本例子主要展示异步执行的逻辑，和取消执行的操作
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_cancel() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    status += pipeline->registerGElement<MyNode1>(&b, {a}, "nodeB");
    status += pipeline->registerGElement<MyNode1>(&c, {b}, "nodeC");
    status += pipeline->registerGElement<MyNode2>(&d, {c}, "nodeD");
    if (!status.isOK()) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定。今后tutorial例子中省略该操作。
    }

    pipeline->init();

    // 异步执行，并且等待结束
    auto result = pipeline->asyncRun();
    CGRAPH_ECHO("pipeline async run first time, BEGIN.");
    result.wait();
    CGRAPH_ECHO("pipeline async run first time, FINISH.");
    CGRAPH_ECHO("======================");

    result = pipeline->asyncRun();
    CGRAPH_ECHO("pipeline async run second time, BEGIN.");
    CGRAPH_SLEEP_MILLISECOND(1500)

    /**
     * pipeline整体流程，正常执行需要4000ms
     * 等1500ms之后，手动cancel一下
     */
    pipeline->cancel();
    CGRAPH_ECHO("pipeline async run second time, CANCEL.");
    CGRAPH_ECHO("======================");

    result.wait();    // 请务必等待执行完成之后，再进行destroy逻辑
    pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


int main () {
    tutorial_cancel();
    return 0;
}

