/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-functional-02.cpp
@Time: 2023/12/27 23:30
@Desc: 
***************************/

#include "../_Materials/TestInclude.h"

using namespace CGraph;

void test_functional_02() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    CStatus status;
    const int runTimes = 5000;

    GElementPtr a,b,c,d,e,f,g,h,i,j,k,l,m,n = nullptr;
    GElementPtr region1, region2, cluster1, cluster2 = nullptr;

    status += pipeline->registerGElement<TestAdd1GNode>(&a, {}, "a");

    b = pipeline->createGNode<TestAdd1GNode>(GNodeInfo({}, "b"));
    c = pipeline->createGNode<TestAdd1GNode>(GNodeInfo({}, "c"));

    d = pipeline->createGNode<TestAdd1GNode>(GNodeInfo({}, "d"));
    e = pipeline->createGNode<TestAdd1GNode>(GNodeInfo("e", 3));
    f = pipeline->createGNode<TestAdd1GNode>(GNodeInfo("f"));
    cluster1 = pipeline->createGGroup<GCluster>({e, f}, {d}, "cluster1");

    g = pipeline->createGNode<TestAdd1GNode>(GNodeInfo({d}, "g"));
    region2 = pipeline->createGGroup<GRegion>({d, cluster1, g}, {}, "region2", 2);

    region1 = pipeline->createGGroup<GRegion>({b, c, region2});

    i = pipeline->createGNode<TestAdd1GNode>(GNodeInfo("i"));
    j = pipeline->createGNode<TestAdd1GNode>(GNodeInfo("j"));
    k = pipeline->createGNode<TestAdd1GNode>(GNodeInfo("k"));
    cluster2 = pipeline->createGGroup<GCluster>({i, j, k}, {a, region1}, "cluster2");

    status += pipeline->registerGElement<GRegion>(&region1, {}, "region1", 3);
    status += pipeline->registerGElement<TestAdd1GNode>(&h, {region1}, "h");
    status += pipeline->registerGElement<GCluster>(&cluster2, {a, region1}, "cluster2");
    status += pipeline->registerGElement<TestAdd1GNode>(&l, {a}, "l");

    status += pipeline->registerGElement<TestAdd1GNode>(&m, {h, cluster2}, "m");
    status += pipeline->registerGElement<TestAdd1GNode>(&n, {l, cluster2}, "n");

    {
        UTimeCounter counter("test_functional_02");
        status += pipeline->process(runTimes);
    }

    if (status.isErr()) {
        std::cout << status.getInfo() << std::endl;
    }

    if (g_test_node_cnt != (runTimes * 50)) {
        std::cout << "test_functional_02: g_test_node_cnt is not right : " << g_test_node_cnt << std::endl;
    }

    GPipelineFactory::remove(pipeline);
}


int main() {
    test_functional_02();
    return 0;
}
