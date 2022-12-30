/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T16-Message.cpp
@Time: 2022/11/3 23:48
@Desc: 本例子主要展示，通过 GMessageParam 在pipeline之间传递参数
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGNode/MySendMessageNode.h"
#include "MyGNode/MyRecvMessageNode.h"

using namespace CGraph;

void send_message() {
    CStatus status;
    GElementPtr a, b;
    GPipelinePtr pubPipeLine = GPipelineFactory::create();

    // 注册用于 发送 的pipeline
    status += pubPipeLine->registerGElement<MyNode1>(&a, {}, "myNode1");
    status += pubPipeLine->registerGElement<MySendMessageNode>(&b, {a});

    pubPipeLine->process(3);
}


void recv_message() {
    CStatus status;
    GElementPtr a, b;
    GPipelinePtr subPipeline = GPipelineFactory::create();

    // 注册用于 接收 的pipeline
    status += subPipeline->registerGElement<MyNode2>(&a, {}, "myNode2");
    status += subPipeline->registerGElement<MyRecvMessageNode>(&b, {a});

    subPipeline->process(3);
}


void tutorial_message_send_recv() {
    CGRAPH_CREATE_MESSAGE_TOPIC(MyMessageParam, "send-recv", 48)    // 创建一个topic，也在算子中实现创建流程

    std::thread sendThd = std::thread(send_message);
    std::thread recvThd = std::thread(recv_message);
    sendThd.join();
    recvThd.join();

    CGRAPH_CLEAR_MESSAGES();
    GPipelineFactory::clear();
}


int main() {
    tutorial_message_send_recv();
    return 0;
}
