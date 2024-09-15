/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-functional-05.cpp
@Time: 2024/1/6 16:45
@Desc: 
***************************/


#include "../_Materials/TestInclude.h"

using namespace CGraph;

void test_functional_05() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    CStatus status;
    GElementPtr a, b, c, d = nullptr;
    status += pipeline->registerGElement<TestReturnErrorGNode>(&a, {});
    status += pipeline->registerGElement<TestReturnErrorGNode>(&b, {});
    status += pipeline->registerGElement<TestReturnErrorGNode>(&c, {});
    status += pipeline->registerGElement<TestReturnErrorGNode>(&d, {});

    pipeline->init();
    {
        UTimeCounter counter("test_functional_05");
        for (int i = 0; i < 10; i++) {
            status = pipeline->run();
            if (!status.isOK()) {
                CGRAPH_ECHO("%s", status.getInfo().c_str());
            }
        }
    }

    pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


int main() {
    test_functional_05();
    return 0;
}
