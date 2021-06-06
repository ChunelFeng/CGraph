/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test.cpp
@Time: 2021/5/4 4:09 下午
@Desc:
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

void demo() {
    /* 创建图化 */
    GFlow* flow = new GFlow();
    CSTATUS status = STATUS_OK;
    GElementPtr a, b, c, d = nullptr;

    /* 注册节点，其中MyNode1和MyNode2必须为GraphNode的子类，否则无法通过编译。
     * MyNode1中run()执行内容为sleep(1s)
     * MyNode2中run()执行内容为sleep(2s) */
    status = flow->registerGElement<MyNode1>(&a, {}, "nodeA");    // a节点执行，没有任何依赖信息
    if (STATUS_OK != status) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定。本例子中省略
    }
    status = flow->registerGElement<MyNode2>(&b, {a}, "nodeB");    // b节点执行，需要依赖a节点执行完毕
    status = flow->registerGElement<MyNode1>(&c, {a}, "nodeC");
    status = flow->registerGElement<MyNode2>(&d, {b, c}, "nodeD");    // d节点执行，需要依赖b和c节点执行完毕

    /* 图信息初始化，准备开始计算 */
    status = flow->init();

    /* 运行图计算。初始化后，支持多次循环计算 */
    for (int i = 0; i < 3; i++) {
        status = flow->run();
        std::cout << "[CGraph] demo, loop : " << i + 1 << ", and run status = " << status << std::endl;
    }

    /* 图信息逆初始化，准备结束计算 */
    status = flow->deinit();
    delete flow;
}

int main () {
    demo();
    return 0;
}
