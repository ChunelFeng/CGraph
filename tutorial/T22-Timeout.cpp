/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T22-Timeout.cpp
@Time: 2023/8/23 22:26
@Desc: 本例主要演示，设定节点超时(timeout)逻辑，针对group同样可以设置
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_timeout() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    status += pipeline->registerGElement<MyNode1>(&b, {a}, "nodeB");
    status += pipeline->registerGElement<MyNode2>(&c, {b}, "nodeC");
    status += pipeline->registerGElement<MyNode1>(&d, {c}, "nodeD");

    status = pipeline->process();    // 正常运行
    if (status.isOK()) {
        std::cout << "pipeline run finish" << std::endl;
    }
    std::cout << "===================" << std::endl;

    /**
     * c 正常执行需要 [2000ms]，但是设置超时时长为 [300 ms]
     * 故在执行的过程中，会出现超时异常。
     * 如果设定为 HOLD_BY_PIPELINE，
     * pipeline会确保在 pipeline->run() 执行完成之前，所有超时节点执行结束
     */
    CMSec timeout = 300;
    std::cout << "set [" << c->getName() << "] timeout as [" << timeout << "]ms" << std::endl;
    c->setTimeout(timeout, GElementTimeoutStrategy::AS_ERROR);
    status = pipeline->process();
    if (!status.isOK()) {
        // 会报超时的错误
        std::cout << "**** T22-timeout pipeline run error info : " << status.getInfo() << std::endl;
    }

    GPipelineFactory::remove(pipeline);
}


int main () {
    tutorial_timeout();
    return 0;
}
