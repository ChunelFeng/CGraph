/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test-functional-04.cpp
@Time: 2024/1/1 00:53
@Desc: 
***************************/


#include "../_Materials/TestInclude.h"

using namespace CGraph;

void test_functional_04() {
    const int halfArrSize = 32;
    const int runTimes = 100000;
    CGRAPH_CREATE_MESSAGE_TOPIC(TestGMessageParam, g_test_message_key, 100)
    std::unique_ptr<TestGMessageParam> mp(new TestGMessageParam());
    CGRAPH_SEND_MPARAM(TestGMessageParam, g_test_message_key, mp, GMessagePushStrategy::WAIT)

    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr arr[halfArrSize * 2];    // 前面一半是串行的，后面一半是并行的
    GElementPtrSet linearSet;

    CStatus status = pipeline->registerGElement<TestRecvMessageGNode>(&arr[0]);
    linearSet.insert(arr[0]);
    for (int i = 1; i < halfArrSize; i++) {
        status += pipeline->registerGElement<TestRecvMessageGNode>(&arr[i], {arr[i-1]});
        linearSet.insert(arr[i]);
    }

    for (int j = halfArrSize; j < halfArrSize * 2; j++) {
        status += pipeline->registerGElement<TestRecvMessageGNode>(&arr[j], linearSet);
    }

    {
        UTimeCounter counter("test_functional_04");
        status += pipeline->process(runTimes);
    }

    status += CGRAPH_RECV_MPARAM_WITH_TIMEOUT(TestGMessageParam, g_test_message_key, mp, 1500)
    if (mp->num_ != halfArrSize * runTimes * 2) {
        CGRAPH_ECHO("result num is wrong, num is [%lu]", mp->num_);
    }

    CGRAPH_CLEAR_MESSAGES()
    GPipelineFactory::clear();
}


int main() {
    test_functional_04();
    return 0;
}
