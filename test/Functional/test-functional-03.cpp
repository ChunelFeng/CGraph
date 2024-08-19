/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-functional-03.cpp
@Time: 2023/12/29 22:13
@Desc: 
***************************/


#include "../_Materials/TestInclude.h"

using namespace CGraph;

void test_functional_03() {
    CStatus status;
    GPipelinePtr pipeline = GPipelineFactory::create();
    const int runTimes = 100000;

    GElementPtr a, b_cluster, c, d_region, e = nullptr;

    b_cluster = pipeline->createGGroup<GCluster>({
        pipeline->createGNode<TestAdd1GNode>(GNodeInfo("nodeB1", 1)),    // 创建名为nodeB1的node信息，并将其放入b_cluster中
        pipeline->createGNode<TestAdd1GNode>(GNodeInfo("nodeB2", 3)),    // 创建名为nodeB2且自循环3次的node信息，并将其放入b_cluster中
        pipeline->createGNode<TestAdd1GNode>(GNodeInfo("nodeB3", 1))
    });

    GElementPtr d1, d2, d3, d4, d23_cluster = nullptr;
    d1 = pipeline->createGNode<TestAdd1GNode>(GNodeInfo({}, "nodeD1", 1));
    d2 = pipeline->createGNode<TestAdd1GNode>(GNodeInfo("nodeD2", 1));    // 创建node，稍后放入cluster中
    d3 = pipeline->createGNode<TestAdd1GNode>(GNodeInfo("nodeD3", 1));
    d23_cluster = pipeline->createGGroup<GCluster>({d2, d3}, {d1}, "clusterD23", 1);
    d4 = pipeline->createGNode<TestAdd1GNode>(GNodeInfo({d1}, "nodeD4", 1));
    d_region = pipeline->createGGroup<GRegion>({d1, d23_cluster, d4});    // 创建名为d_region的region信息，并将{d1,d23_cluster,d4}放入其中

    status += pipeline->registerGElement<TestAdd1GNode>(&a, {}, "nodeA", 1);
    status += pipeline->registerGGroup(&b_cluster, {}, "clusterB", 1);
    status += pipeline->registerGElement<TestAdd1GNode>(&c, {a, b_cluster}, "nodeC", 1);
    status += pipeline->registerGGroup(&d_region, {a, b_cluster}, "regionD", 2);    // 将名为regionD，依赖{a,b_cluster}执行且自循环2次的region信息，注册入pipeline中
    status += pipeline->registerGElement<TestAdd1GNode>(&e, {c, d_region}, "nodeE", 1);
    pipeline->addGAspect<TestMaterialAdd1GAspect>();

    if (!status.isOK()) {
        return;
    }

    {
        UTimeCounter counter("test_functional_03");
        status = pipeline->process(runTimes);
    }

    if (status.isErr()) {
        std::cout << status.getInfo() << std::endl;
    }

    if (g_test_node_cnt != (runTimes * 58)) {
        std::cout << "test_functional_03: g_test_node_cnt is not right : " << g_test_node_cnt << std::endl;
    }

    GPipelineFactory::remove(pipeline);
}


int main() {
    test_functional_03();
    return 0;
}
