/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-functional-05.cpp
@Time: 2025/3/14 23:10
@Desc: 
***************************/


#include "../_Materials/TestInclude.h"

using namespace CGraph;

void test_functional_05() {
    CStatus status;
    GPipelinePtr pipeline = GPipelineFactory::create();

    GElementPtr a, b_cluster, c, d_region, e = nullptr;

    b_cluster = pipeline->createGGroup<GCluster>({
                     pipeline->createGNode<TestAdd1ByParamGNode>(GNodeInfo("nodeB1", 1)),    // 创建名为nodeB1的node信息，并将其放入b_cluster中
                     pipeline->createGNode<TestAdd1ByParamGNode>(GNodeInfo("nodeB2", 3)),    // 创建名为nodeB2且自循环3次的node信息，并将其放入b_cluster中
                     pipeline->createGNode<TestAdd1ByParamGNode>(GNodeInfo("nodeB3", 1))
                 });

    GElementPtr d1, d2, d3, d4, d23_cluster = nullptr;
    d1 = pipeline->createGNode<TestAdd1ByParamGNode>(GNodeInfo({}, "nodeD1", 1));
    d2 = pipeline->createGNode<TestAdd1ByParamGNode>(GNodeInfo("nodeD2", 1));    // 创建node，稍后放入cluster中
    d3 = pipeline->createGNode<TestAdd1ByParamGNode>(GNodeInfo("nodeD3", 1));
    d23_cluster = pipeline->createGGroup<GCluster>({d2, d3}, {d1}, "clusterD23", 1);
    d4 = pipeline->createGNode<TestAdd1ByParamGNode>(GNodeInfo({d1}, "nodeD4", 1));
    d_region = pipeline->createGGroup<GRegion>({d1, d23_cluster, d4});    // 创建名为d_region的region信息，并将{d1,d23_cluster,d4}放入其中

    status += pipeline->registerGElement<TestAdd1ByParamGNode>(&a, {}, "nodeA", 1);
    status += pipeline->registerGGroup(&b_cluster, {}, "clusterB", 1);
    status += pipeline->registerGElement<TestAdd1ByParamGNode>(&c, {a, b_cluster}, "nodeC", 1);
    status += pipeline->registerGGroup(&d_region, {a, b_cluster}, "regionD", 2);    // 将名为regionD，依赖{a,b_cluster}执行且自循环2次的region信息，注册入pipeline中
    status += pipeline->registerGElement<TestAdd1ByParamGNode>(&e, {c, d_region}, "nodeE", 1);

    pipeline->addGAspect<TestAdd1ByParamGAspect>();
    pipeline->createGParam<TestAdd1GParam>(g_add1_by_param_key);

    pipeline->init();
    {
        UTimeCounter counter("test_functional_05");
        pipeline->run();
    }

    auto p = pipeline->getGParam<TestAdd1GParam>(g_add1_by_param_key);
    if (37 != p->cnt_) {
        CGRAPH_ECHO("test-functional-05 run error, cnt is [%d]", p->cnt_);
    }

    pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


int main() {
    test_functional_05();
    return 0;
}
