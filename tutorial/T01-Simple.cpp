/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T01-Simple.cpp
@Time: 2021/5/4 4:09 下午
@Desc: 本例主要演示，GPipeline注册和添加依赖关系的流程
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_simple() {
    /* 创建图对应的pipeline */
    GPipelinePtr pipeline = GPipelineFactory::create();

    /* 定义GElementPtr类型的变量 */
    GElementPtr a, b, c, d = nullptr;

    /**
     * 注册节点，其中MyNode1和MyNode2必须为GNode的子类，否则无法通过编译。
     * status+= 操作，可以用于记录链路异常问题
     * */
    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");    // 将名为nodeA，无执行依赖的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");    // 将名为nodeB，依赖a执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    status += pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");    // 将名为nodeD，依赖{b,c}执行的node信息，注册入pipeline中
    if (!status.isOK()) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定。今后tutorial例子中省略该操作。
    }

    /* 图信息初始化，准备开始计算 */
    status += pipeline->init();

    /* 运行流图信息。初始化后，支持多次循环计算 */
    for (int i = 0; i < 3; i++) {
        status += pipeline->run();
        CGRAPH_ECHO("==== tutorial_simple, loop : [%d], and run status = [%d].", i + 1, status.getCode());
    }

    /* 图信息逆初始化，准备结束计算 */
    status += pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


int main () {
    tutorial_simple();
    return 0;
}
