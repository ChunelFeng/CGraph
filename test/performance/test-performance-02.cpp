/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-performance-02.cpp
@Time: 2023/12/3 18:28
@Desc: 
***************************/

#include "../_materials/TestNodes.h"

using namespace CGraph;

void tutorial_serial_32() {
    // 串行执行32次，对应第二个例子，1thread，32串行，1000w次
    GPipelinePtr pipeline = GPipelineFactory::create();
    CStatus status;
    GElementPtr arr[32];
    pipeline->registerGElement<TestMaterialAdd1Node>(&arr[0]);
    for (int i = 1; i < 32; i++) {
        pipeline->registerGElement<TestMaterialAdd1Node>(&arr[i], {arr[i-1]});
    }
    pipeline->makeSerial();
    pipeline->setAutoCheck(false);
    status += pipeline->init();
    /** 其中流程进行计时 **/
    {
        UTimeCounter counter;
        for (int t = 0; t < 1000000; t++) {
            pipeline->run();
        }
    }

    status += pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}

int main() {
    tutorial_serial_32();
    return 0;
}
