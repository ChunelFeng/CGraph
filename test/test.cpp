/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: test.cpp
@Time: 2021/5/4 4:09 下午
@Desc: 
***************************/

#include "MyGraphNode/MyNode1.h"
#include "MyGraphNode/MyNode2.h"

/* 在正式编程中，请对所有new对象的过程做异常处理
 * 并且对所有接口的返回值做判断 */
void demo() {
    /* 创建图化 */
    Graphic* graphic = new Graphic();
    CSTATUS status = STATUS_OK;

    /* 创建节点，其中MyNode1和MyNode2必须为GraphNode的子类，否则无法通过编译。
     * MyNode1中run()执行内容为sleep(1s)
     * MyNode2中run()执行内容为sleep(2s) */
    GraphNode* a = graphic->createGraphNode<MyNode1>();
    if (nullptr == a) {
        return;
    }
    GraphNode* b = graphic->createGraphNode<MyNode2>();
    GraphNode* c = graphic->createGraphNode<MyNode1>();
    GraphNode* d = graphic->createGraphNode<MyNode2>();

    status = b->addDependNode(a);    // b节点必须在a节点之后执行
    if (STATUS_OK != status) {
        return;
    }
    c->addDependNode(a);    // c节点必须在a节点之后执行
    d->addDependNode(b);    // d节点必须在b和c节点执行之后执行
    d->addDependNode(c);

    /* 图信息初始化，准备开始计算 */
    status = graphic->init();

    /* 运行图计算 */
    status = graphic->run();

    /* 图信息逆初始化，准备结束计算 */
    status = graphic->deinit();
    delete graphic;

    return;
}

int main () {
    demo();
    return 0;
}