/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T21-MultiCondition.cpp
@Time: 2023/8/18 21:57
@Desc: 本例子主要展示 多条件(组) 的执行逻辑
***************************/

#include "MyGNode/MyMatchNode.h"
#include "MyGNode/MyNode1.h"
#include "MyGNode/MyWriteParamNode.h"

using namespace CGraph;

void tutorial_multi_condition() {
    GPipelinePtr pipeline = GPipelineFactory::create();    // 创建一个pipeline，用于执行图框架信息
    GElementPtr a, b_multi_condition, c, d_multi_condition = nullptr;
    b_multi_condition = pipeline->createGGroup<GMultiCondition<GMultiConditionType::SERIAL>>({
        pipeline->createGNode<MyNode1>(GNodeInfo("nodeB1")),    // 本节点【不会】执行，因为默认不执行
        pipeline->createGNode<MyMatchNode>(GNodeInfo("nodeB2"))    // 本节点【会】执行，因为 isMatch() 返回为 true
    });

    d_multi_condition = pipeline->createGGroup<GMultiCondition<GMultiConditionType::PARALLEL>>({
        // 这里的两个节点，均不会被执行，以为 isMatch 的条件不满足
        pipeline->createGNode<MyMatchNode>(GNodeInfo("nodeD1")),
        pipeline->createGNode<MyMatchNode>(GNodeInfo("nodeD2"))
    });

    CStatus status = pipeline->registerGElement<MyWriteParamNode>(&a, {}, "nodeA");
    // 新版本中，推荐使用 registerGGroup 来注册group信息，简化流程
    status += pipeline->registerGGroup(&b_multi_condition, {a}, "multiConditionB");
    status += pipeline->registerGElement<MyWriteParamNode>(&c, {b_multi_condition}, "nodeC");
    status += pipeline->registerGGroup(&d_multi_condition, {c}, "multiConditionD");

    pipeline->process();    // 运行pipeline
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_multi_condition();
    return 0;
}
