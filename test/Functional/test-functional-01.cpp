/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-functional-01.cpp
@Time: 2023/12/27 23:16
@Desc: 
***************************/

#include "../_Materials/TestGNodes.h"

using namespace CGraph;

void test_functional_01() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    CStatus status;
    GElementPtr a, b, c, d, e, f, g, h, i, j = nullptr;
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&a, {});
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&b, {});
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&c, {a});
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&d, {b});
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&e, {b, c});
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&f, {c});
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&g, {d, e, f});
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&h, {f});
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&i, {g, h});
    status += pipeline->registerGElement<TestMaterialAdd1GNode>(&j, {h});

    status = pipeline->process(1000000);
    if (status.isErr()) {
        std::cout << status.getInfo() << std::endl;
    }

    GPipelineFactory::remove(pipeline);
}


int main() {
    test_functional_01();
    return 0;
}
