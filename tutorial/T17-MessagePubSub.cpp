/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T17-Message-PubSub.cpp
@Time: 2022/12/27 00:19
@Desc: 本例子主要展示，通过 GMessageParam 在pipeline之间通过sub pub的方式传递
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGNode/MyPubMessageNode.h"
#include "MyGNode/MySubMessageNode.h"

using namespace CGraph;

void sub_message() {
    CStatus status;
    GElementPtr node1, pubNode;
    GPipelinePtr pubPipeLine = GPipelineFactory::create();

    // 注册用于 发布 的pipeline
    status += pubPipeLine->registerGElement<MyNode1>(&node1, {}, "myNode1");
    status += pubPipeLine->registerGElement<MyPubMessageNode>(&pubNode, {});

    pubPipeLine->process(5);
}


void pub_message() {
    CStatus status;
    GElementPtr node2;
    GTemplateNodePtr<int> subNode;
    GPipelinePtr subPipeline = GPipelineFactory::create();
    // 每次调用bind方法的时候，会返回不同的 connId信息
    int connId = CGRAPH_BIND_MESSAGE_TOPIC(MyMessageParam, "pub-sub", 1024)

    // 注册用于 订阅 的pipeline
    status += subPipeline->registerGElement<MyNode2>(&node2, {}, "myNode2");
    status += subPipeline->registerGElement<MySubMessageNode<int>>(&subNode, {node2}, connId);
    subNode->setName("MySubMessageNode");

    subPipeline->process(5);
}


void tutorial_message_pub_sub() {
    // 一个pipeline pub数据，其他pipeline sub相同的数据内容
    std::thread pubThd = std::thread(sub_message);
    std::thread subThd1 = std::thread(pub_message);
    std::thread subThd2 = std::thread(pub_message);
    std::thread subThd3 = std::thread(pub_message);

    pubThd.join();
    subThd1.join();
    subThd2.join();
    subThd3.join();

    CGRAPH_CLEAR_MESSAGES()
    GPipelineFactory::clear();
}


int main() {
    tutorial_message_pub_sub();
    return 0;
}
