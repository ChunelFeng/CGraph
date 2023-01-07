/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T15-EParam.cpp
@Time: 2022/5/29 22:29
@Desc: 本例子主要展示，向 MyEParamNode 节点中，传递特定参数，并在节点执行时候打印的逻辑
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyEParamNode.h"
#include "MyParams/MyConnParam.h"

using namespace CGraph;

void tutorial_element_param() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, v1, v2 = nullptr;

    pipeline->registerGElement<MyNode1>(&a, {}, "node-1");
    /**
     * 注册同样类型（MyEParamNode）的节点，但在下方传入不同类型的参数（EParam）
     * 从而执行不同的结果
     */
    pipeline->registerGElement<MyEParamNode>(&v1, {a}, "version-1");
    pipeline->registerGElement<MyEParamNode>(&v2, {v1}, "version-2");

    MyVersionParam vp1;
    vp1.priority_ = 1;
    vp1.secondary_ = 1;
    /**
     * 往v1中，传入参数特定参数
     */
    v1->addEParam(VERSION_PARAM_KEY, &vp1);

    /**
     * 可以根据需要，传入不同 key 值的不同类型的参数
     * 被传入的参数类，需要是GPassedParam类（或其衍生类）的子类
     */
    MyVersionParam vp2;
    vp2.priority_ = 2;
    vp2.secondary_ = 2;
    v2->addEParam(VERSION_PARAM_KEY, &vp2);

    MyConnParam cp;
    cp.ip_ = "127.0.0.1";
    cp.port_ = 8080;
    v2->addEParam(CONN_PARAM_KEY, &cp);

    pipeline->process();    // 运行pipeline
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_element_param();
    return 0;
}
