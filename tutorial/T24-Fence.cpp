/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T24-Fence.cpp
@Time: 2023/9/6 23:17
@Desc: 本例主要演示，通过 GFence逻辑，来控制超时节点的等待逻辑
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_fence() {
    CStatus status;
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr b, c, e = nullptr;
    GFunctionPtr a_function = nullptr;
    GFencePtr d_fence = nullptr;    // 通过 fence，去等待前方指定的异步节点执行完成的逻辑

    status += pipeline->registerGElement<GFunction>(&a_function, {}, "functionA");
    status += pipeline->registerGElement<MyNode2>(&b, {a_function}, "nodeB");
    status += pipeline->registerGElement<MyNode1>(&c, {a_function}, "nodeC");
    status += pipeline->registerGElement<GFence>(&d_fence, {b, c}, "fenceD");
    status += pipeline->registerGElement<MyNode1>(&e, {d_fence}, "nodeE");

    CSec sec = 5;
    a_function->setFunction(CFunctionType::RUN, [a_function, sec] {
        CGRAPH_ECHO("[%s] begin sleep for [%ld]s", a_function->getName().c_str(), sec);
        CGRAPH_SLEEP_SECOND(sec)
        CGRAPH_ECHO("[%s] run finished.", a_function->getName().c_str());
        return CStatus();
    });

    // 设定一个执行 5s的功能，并且让其运行200就进入超时，故后面的 b和c，可以运行起来了
    a_function->setTimeout(200, GElementTimeoutStrategy::HOLD_BY_PIPELINE);
    b->setTimeout(200, GElementTimeoutStrategy::HOLD_BY_PIPELINE);

    /**
     * 在这里添加一个栅栏，目的是为了等待 a_function, b 都执行完成之后，后面的逻辑才可以开始
     * fence中加入的，必须是异步执行（设置 timeout耗时）的element
     * 设定为 HOLD_BY_PIPELINE 的element，如果不加入 fence中，则在 pipeline run结束之前，会等待其执行结束
     */
    d_fence->waitGElements({a_function, b});

    status = pipeline->process();
    if (status.isErr()) {
        std::cout << status.getInfo() << std::endl;
    }

    GPipelineFactory::remove(pipeline);
}


int main () {
    tutorial_fence();
    return 0;
}
