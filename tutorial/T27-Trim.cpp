/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T27-Trim.cpp
@Time: 2024/9/28 21:43
@Desc: 本例主要演示，通过 trim() 方法，修剪pipeline中的冗余依赖逻辑
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_trim() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");
    pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");

    /**
     * 可以看出，d节点 对a 的依赖，是可有可无的
     * 建议通过 trim() 接口删除冗余依赖
     * 参考文档：http://www.chunel.cn/archives/cgraph-remove-redundancy-link
     */
    pipeline->registerGElement<MyNode2>(&d, {a, b, c}, "nodeD");

    auto trimSize = pipeline->trim();
    CGRAPH_ECHO("trim size is: %d", (int)trimSize);

    // 查看 trim后的效果
    pipeline->dump();
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_trim();
    return 0;
}
