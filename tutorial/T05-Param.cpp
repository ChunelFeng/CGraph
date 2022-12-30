/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T05-Param.cpp
@Time: 2021/6/13 3:31 下午
@Desc: 本例主要演示，参数传递功能
***************************/

#include "MyGNode/MyWriteParamNode.h"
#include "MyGNode/MyReadParamNode.h"

using namespace CGraph;

void tutorial_param() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    CStatus status;
    GElementPtr a, b, c, d, e, f = nullptr;

    status += pipeline->registerGElement<MyReadParamNode>(&a, {}, "readNodeA");     // 读取param中的信息，不做修改
    status += pipeline->registerGElement<MyReadParamNode>(&b, {a}, "readNodeB");
    status += pipeline->registerGElement<MyWriteParamNode>(&c, {a}, "writeNodeC");
    status += pipeline->registerGElement<MyWriteParamNode>(&d, {a}, "writeNodeD", 2);    // 对param中的iValue值+1，循环执行2次
    status += pipeline->registerGElement<MyReadParamNode>(&e, {a}, "readNodeE");
    status += pipeline->registerGElement<MyWriteParamNode>(&f, {b, c, d, e}, "writeNodeF");
    if (!status.isOK()) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定。本例子中省略
    }

    status = pipeline->init();

    for (int i = 0; i < 3; i++) {
        status = pipeline->run();
        std::cout << "[CGraph] tutorial_param, loop : " << i + 1 << ", and run status = " << status.getCode() << std::endl;
    }

    status = pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_param();
    return 0;
}

