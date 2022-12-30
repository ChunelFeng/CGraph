/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T11-Singleton.cpp
@Time: 2021/10/30 10:32 下午
@Desc: 本例主要展示，通过添加GSingleton添加的节点，均是同一个节点（指针地址相同）
***************************/

#include "MyGNode/MyShowAddressNode.h"
#include "MyGNode/MyNode1.h"

using namespace CGraph;

#if __cplusplus >= 201703L

void tutorial_singleton() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d, e = nullptr;

    /* 通过GSingleton方式注册的节点，地址相同 */
    pipeline->registerGElement<GSingleton<MyShowAddressNode>>(&a, {}, "singleton", 2);

    /* 通过普通方式注册的节点，地址不同 */
    pipeline->registerGElement<MyShowAddressNode>(&b, {a}, "not singleton");
    pipeline->registerGElement<GSingleton<MyShowAddressNode>>(&c, {a});    // c的地址和a是相同的
    pipeline->registerGElement<GSingleton<MyNode1>>(&d, {c}, "nodeD");
    pipeline->registerGElement<GSingleton<MyShowAddressNode>>(&e, {c});    // e的地址和a也是相同的

    pipeline->process();
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_singleton();
    return 0;
}

#else

int main() {
    CGRAPH_ECHO("[warning] tutorial_singleton only support C++17 or upper version.");
    return 0;
}

#endif