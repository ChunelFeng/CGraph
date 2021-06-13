/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T05-Param.cpp
@Time: 2021/6/13 3:31 下午
@Desc: 
***************************/

#include "MyGNode/MyParamNode1.h"
#include "MyGNode/MyParamNode2.h"


void tutorial_param() {
    GPipelinePtr pipeline = new GPipeline();
    CSTATUS status = STATUS_OK;
    GElementPtr a, b, c, d = nullptr;

    status = pipeline->registerGElement<MyParamNode1>(&a, {}, "paramNodeA");    // 将名为nodeA，无执行依赖的node信息，注册入pipeline中
    if (STATUS_OK != status) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定。本例子中省略
    }
    status = pipeline->registerGElement<MyParamNode2>(&b, {a}, "paramNodeB", 9000000);    // 将名为nodeB，依赖a执行的node信息，注册入pipeline中
    status = pipeline->registerGElement<MyParamNode1>(&c, {a}, "paramNodeC", 9000000);
    status = pipeline->registerGElement<MyParamNode2>(&d, {b, c}, "paramNodeD");    // 将名为nodeD，依赖{b,c}执行的node信息，注册入pipeline中

    /* 图信息初始化，准备开始计算 */
    status = pipeline->init();

    /* 运行图计算。初始化后，支持多次循环计算 */

    for (int i = 0; i < 1; i++) {
        status = pipeline->run();
    }

    /* 图信息逆初始化，准备结束计算 */
    status = pipeline->deinit();
    delete pipeline;
}

int main() {
    tutorial_param();
    return 0;
}

