/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T05-Param.cpp
@Time: 2021/6/13 3:31 下午
@Desc: 
***************************/

#include "MyGNode/MyWriteParamNode.h"
#include "MyGNode/MyReadParamNode.h"


void tutorial_param() {
    GPipelinePtr pipeline = new GPipeline();
    CSTATUS status = STATUS_OK;
    GElementPtr a, b, c, d, e, f = nullptr;

    status = pipeline->registerGElement<MyReadParamNode>(&a, {}, "readNodeA");     // 读取param中的信息，不做修改
    if (STATUS_OK != status) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定。本例子中省略
    }
    status = pipeline->registerGElement<MyReadParamNode>(&b, {a}, "readNodeB");
    status = pipeline->registerGElement<MyWriteParamNode>(&c, {a}, "writeNodeC");
    status = pipeline->registerGElement<MyWriteParamNode>(&d, {a}, "writeNodeD", 2);    // 对param中的iValue值+1，循环执行2次
    status = pipeline->registerGElement<MyReadParamNode>(&e, {a}, "readNodeE");
    status = pipeline->registerGElement<MyWriteParamNode>(&f, {b, c, d, e}, "writeNodeF");

    /* 运行pipeline信息 */
    status = pipeline->process();
    std::cout << "[CGraph] tutorial_param process status = " << status << std::endl;
    delete pipeline;
}

int main() {
    tutorial_param();
    return 0;
}

