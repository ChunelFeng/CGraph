/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T02-Cluster.cpp
@Time: 2021/6/7 8:51 下午
@Desc: 
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

void tutorial_cluster () {
    CSTATUS status = STATUS_OK;
    GPipelinePtr pipeline = new GPipeline();
    GElementPtr a, b_cluster, c, d = nullptr;

    b_cluster = pipeline->createGNodeS<GCluster>({
         pipeline->createGNode<MyNode1>(GNodeInfo("nodeB1", 1)),    // 创建名为nodeB1的node信息，并将其放入b_cluster中
         pipeline->createGNode<MyNode1>(GNodeInfo("nodeB2", 3)),    // 创建名为nodeB2且自循环3次的node信息，并将其放入b_cluster中
         pipeline->createGNode<MyNode2>(GNodeInfo("nodeB3", 1))
    });    // 创建cluster信息，包含了三个node信息

    if (nullptr == b_cluster) {
        return;
    }

    /* 正式使用时，请对所有返回值进行判定 */
    status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA", 1);        // 将名为nodeA的node信息，注册入pipeline中
    if (STATUS_OK != status) {
        return;
    }
    status = pipeline->registerGElement<GCluster>(&b_cluster, {a}, "clusterB", 2);    // 将名为clusterB，依赖a执行且自循环2次的cluster信息，注册入pipeline中
    status = pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC", 1);
    status = pipeline->registerGElement<MyNode2>(&d, {b_cluster, c}, "nodeD", 2);

    status = pipeline->process();
    CGRAPH_ECHO("pipeline process status is : [%d]", status);

    delete pipeline;
}

int main () {
    tutorial_cluster();
    return 0;
}
