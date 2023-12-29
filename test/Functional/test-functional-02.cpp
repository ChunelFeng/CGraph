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
    GElementPtr a,b,c,d,e,f,g,h,i,j,k,l,m,n = nullptr;
    GElementPtr region1, region2, cluster1, cluster2 = nullptr;

    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&a, {}, "a");

    b = pipeline->createGNode<TestMaterialAdd1GNode>(GNodeInfo({}, "b"));
    c = pipeline->createGNode<TestMaterialAdd1GNode>(GNodeInfo({}, "c"));

    d = pipeline->createGNode<TestMaterialAdd1GNode>(GNodeInfo({}, "d"));
    e = pipeline->createGNode<TestMaterialAdd1GNode>(GNodeInfo("e", 3));
    f = pipeline->createGNode<TestMaterialAdd1GNode>(GNodeInfo("f"));
    cluster1 = pipeline->createGGroup<GCluster>({e, f}, {d}, "cluster1");

    g = pipeline->createGNode<TestMaterialAdd1GNode>(GNodeInfo({d}, "g"));
    region2 = pipeline->createGGroup<GRegion>({d, cluster1, g}, {}, "region2", 2);

    region1 = pipeline->createGGroup<GRegion>({b, c, region2});

    i = pipeline->createGNode<TestMaterialAdd1GNode>(GNodeInfo("i"));
    j = pipeline->createGNode<TestMaterialAdd1GNode>(GNodeInfo("j"));
    k = pipeline->createGNode<TestMaterialAdd1GNode>(GNodeInfo("k"));
    cluster2 = pipeline->createGGroup<GCluster>({i, j, k}, {a, region1}, "cluster2");

    status += pipeline->registerGElement<GRegion>(&region1, {}, "region1", 3);
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&h, {region1}, "h");
    status += pipeline->registerGElement<GCluster>(&cluster2, {a, region1}, "cluster2");
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&l, {a}, "l");

    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&m, {h, cluster2}, "m");
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&n, {l, cluster2}, "n");

    {
        UTimeCounter counter("test_functional_02");
        status = pipeline->process(10000);
    }

    if (status.isErr()) {
        std::cout << status.getInfo() << std::endl;
    }
    GPipelineFactory::remove(pipeline);
}


int main() {
    test_functional_02();
    return 0;
}
