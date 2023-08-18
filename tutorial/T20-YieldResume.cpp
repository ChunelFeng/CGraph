/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T20-YieldResume.cpp
@Time: 2023/6/24 15:08
@Desc: 本例子主要展示异步执行的逻辑中，如何暂停和恢复执行pipeline的功能
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_yield_resume() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    status += pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB", 3);
    status += pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC", 3);
    status += pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");
    if (!status.isOK()) {
        std::cout << status.getInfo() << std::endl;
        return;    // 使用时，请对所有CGraph接口的返回值做判定。今后tutorial例子中省略该操作。
    }

    status += pipeline->init();

    CGRAPH_ECHO("pipeline async run, BEGIN.");
    auto result = pipeline->asyncRun();
    CGRAPH_SLEEP_MILLISECOND(2600)

    CGRAPH_ECHO("pipeline async run, YIELD.");
    status += pipeline->yield();    // 暂停执行，保留当前pipeline内部所有参数信息和状态信息

    CGRAPH_SLEEP_MILLISECOND(7200)
    CGRAPH_ECHO("pipeline async run, RESUME after 7200ms.");
    status += pipeline->resume();     // 暂停一段时间后，恢复执行

    result.wait();    // 等待pipeline异步执行结束
    pipeline->destroy();

    GPipelineFactory::remove(pipeline);
}


int main () {
    tutorial_yield_resume();
    return 0;
}
