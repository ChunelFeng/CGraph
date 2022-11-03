/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T16-Message.cpp
@Time: 2022/11/3 23:48
@Desc: 本例子主要展示，通过 GMessageParam 在pipeline之间传递参数
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGNode/MyPubMessageNode.h"
#include "MyGNode/MySubMessageNode.h"

using namespace CGraph;

void pub_message() {
    CStatus status;
    GElementPtr a, b;
    GPipelinePtr pubPipeLine = GPipelineFactory::create();

    // 注册用于发布的pipeline
    status += pubPipeLine->registerGElement<MyNode1>(&a, {}, "myNode1");
    status += pubPipeLine->registerGElement<MyPubMessageNode>(&b, {a});

    pubPipeLine->process(3);
}


void sub_message() {
    CStatus status;
    GElementPtr a, b;
    GPipelinePtr subPipeline = GPipelineFactory::create();

    // 注册用于订阅的pipeline
    status += subPipeline->registerGElement<MyNode2>(&a, {}, "myNode2");
    status += subPipeline->registerGElement<MySubMessageNode>(&b, {a});

    subPipeline->process(3);
}


void tutorial_message() {
    CGRAPH_CREATE_MESSAGE_TOPIC(MyMessageParam, "test", 10);    // 创建一个topic，也在算子中实现创建流程

    std::thread pubThd = std::thread(pub_message);
    std::thread subThd = std::thread(sub_message);
    pubThd.join();
    subThd.join();

    CGRAPH_CLEAR_MESSAGES();
    GPipelineFactory::clear();
}


int main() {
    tutorial_message();
    return 0;
}
