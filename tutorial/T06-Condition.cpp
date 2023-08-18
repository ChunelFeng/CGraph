/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T06-Condition.cpp
@Time: 2021/6/19 10:44 下午
@Desc: 本例主要演示，根据条件进行逻辑判断的功能
***************************/

#include "MyGCondition/MyCondition.h"
#include "MyGCondition/MyParamCondition.h"
#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGNode/MyReadParamNode.h"
#include "MyGNode/MyWriteParamNode.h"

using namespace CGraph;

void tutorial_condition() {
    CStatus status;
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b_condition, c, d_condition = nullptr;

    b_condition = pipeline->createGGroup<MyCondition>({
        pipeline->createGNode<MyNode1>(GNodeInfo("conditionNodeB0", 1)),
        pipeline->createGNode<MyNode2>(GNodeInfo("conditionNodeB1", 1)),
        pipeline->createGNode<MyNode1>(GNodeInfo("conditionNodeB2", 1))
    });    // 生成 b_condition。执行的时候，根据choose()的返回值，在B0,B1,B2中选择一个执行

    d_condition = pipeline->createGGroup<MyParamCondition>({
        pipeline->createGNode<MyNode1>(GNodeInfo("paramConditionNodeD0", 1)),
        pipeline->createGNode<MyNode1>(GNodeInfo("paramConditionNodeD1", 1)),
        pipeline->createGNode<MyNode1>(GNodeInfo("paramConditionNodeD2", 1))
    });

    status += pipeline->registerGElement<MyWriteParamNode>(&a, {}, "writeNodeA", 1);
    status += pipeline->registerGElement<MyCondition>(&b_condition, {a}, "conditionB", 1);
    status += pipeline->registerGElement<MyReadParamNode>(&c, {b_condition}, "readNodeC", 1);
    status += pipeline->registerGElement<MyParamCondition>(&d_condition, {c}, "conditionD", 1);
    if (!status.isOK()) {
        return;
    }

    status = pipeline->init();

    for (int i = 0; i < 3; i++) {
        status += pipeline->run();
        std::cout << "[CGraph] tutorial_condition, loop : " << i + 1 << ", and run status = " << status.getCode() << std::endl;
    }

    status = pipeline->destroy();

    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_condition();
    return 0;
}
