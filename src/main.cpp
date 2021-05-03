/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: main.cpp
@Time: 2021/4/26 8:48 下午
@Desc: 
***************************/

#include <iostream>
#include "GraphCtrl/GraphInclude.h"

using namespace std;

int demo() {
    Graphic* graphic = new Graphic();

    GraphNode* a = new GraphNode();    // 自己继承一下这个类，然后实现自己的run函数
    GraphNode* b = new GraphNode();
    GraphNode* c = new GraphNode();
    GraphNode* d = new GraphNode();

    // 添加依赖关系
    b->addDependNode(a);    // b节点必须在a节点之后执行
    c->addDependNode(b);
    c->addDependNode(a);    // c节点必须在a和b节点后执行（上一句add了b）
    d->addDependNode(b);
    d->addDependNode(c);

    // 把 a b c d 节点注册到图化中
    graphic->addGraphNode(a);
    graphic->addGraphNode(b);
    graphic->addGraphNode(c);
    graphic->addGraphNode(d);

    // 初始化所有信息
    graphic->init();

    // 运行图计算
    graphic->run();

    // 逆初始化
    graphic->deinit();
    delete graphic;
}

int main() {
    return demo();
}

