/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T04-Complex.cpp
@Time: 2021/6/7 9:00 下午
@Desc: 
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

void tutorial_complex () {
    CSTATUS status = STATUS_OK;
    GPipelinePtr pipeline = new GPipeline();
    GElementPtr a, b_cluster, c, d_region, e = nullptr;

    b_cluster = pipeline->createGNodeS<GCluster>({
                                                         pipeline->createGNode<MyNode1>(GNodeInfo("nodeB1",
                                                                                                  1)),    // 创建名为nodeB1的node信息，并将其放入b_cluster中
                                                         pipeline->createGNode<MyNode1>(GNodeInfo("nodeB2",
                                                                                                  1)),    // 创建名为nodeB2且自循环3次的node信息，并将其放入b_cluster中
                                                         pipeline->createGNode<MyNode2>(GNodeInfo("nodeB3", 1))
                                                 });

    GElementPtr d1, d2, d3, d4, d23_cluster = nullptr;
    d1 = pipeline->createGNode<MyNode1>(GNodeInfo({}, "nodeD1", 1));
    d2 = pipeline->createGNode<MyNode1>(GNodeInfo("nodeD2", 1));
    d3 = pipeline->createGNode<MyNode1>(GNodeInfo("nodeD3", 1));
    d23_cluster = pipeline->createGNodeS<GCluster>({d2, d3}, {d1}, "clusterD23", 1);
    d4 = pipeline->createGNode<MyNode2>(GNodeInfo({d1}, "nodeD4", 1));
    d_region = pipeline->createGNodeS<GRegion>({d1, d23_cluster, d4});    // 创建名为d_region的region信息，并将{d1,d23_cluster,d4}放入其中

    status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA", 1);
    if (STATUS_OK != status) {
        return;
    }
    status = pipeline->registerGElement<GCluster>(&b_cluster, {}, "clusterB", 1);
    status = pipeline->registerGElement<MyNode1>(&c, {a, b_cluster}, "nodeC", 1);
    status = pipeline->registerGElement<GRegion>(&d_region, {a, b_cluster}, "regionD", 2);    // 将名为regionD，依赖{a,b_cluster}执行且自循环2次的region信息，注册入pipeline中
    status = pipeline->registerGElement<MyNode1>(&e, {c, d_region}, "nodeE", 1);

    status = pipeline->init();
    for (int i = 0; i < 3; i++) {
        status = pipeline->run();
        std::cout << "[CGraph] tutorial_complex, loop : " << i + 1 << ", and run status = " << status << std::endl;
    }
    status = pipeline->deinit();

    return;
}

int main () {
    tutorial_complex();
    return 0;
}