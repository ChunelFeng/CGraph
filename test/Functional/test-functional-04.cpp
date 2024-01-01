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
    const int ARR_SIZE = 128;
    const int RUN_TIMES = 500000;
    CGRAPH_CREATE_MESSAGE_TOPIC(TestGMessageParam, g_test_message_key, 100)
    std::unique_ptr<TestGMessageParam> mp(new TestGMessageParam());
    CGRAPH_SEND_MPARAM(TestGMessageParam, g_test_message_key, mp, GMessagePushStrategy::WAIT)

    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr arr[ARR_SIZE];
    CStatus status = pipeline->registerGElement<TestRecvMessageGNode>(&arr[0]);
    for (int i = 1; i < ARR_SIZE; i++) {
        status += pipeline->registerGElement<TestRecvMessageGNode>(&arr[i], {arr[i-1]});
    }

    {
        UTimeCounter counter("test_functional_04");
        status += pipeline->process(RUN_TIMES);
    }

    status += CGRAPH_RECV_MPARAM_WITH_TIMEOUT(TestGMessageParam, g_test_message_key, mp, 1)
    if (mp->num_ != ARR_SIZE * RUN_TIMES) {
        CGRAPH_ECHO("result num is wrong, num is [%lu]", mp->num_);
    }

    CGRAPH_CLEAR_MESSAGES()
    GPipelineFactory::clear();
}


int main() {
    test_functional_04();
    return 0;
}
