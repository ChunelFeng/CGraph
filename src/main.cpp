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

void func() {
    Graphic* graphic = new Graphic();

    GraphNode* a = new GraphNode();
    GraphNode* b = new GraphNode();
    GraphNode* c = new GraphNode();
    GraphNode* d = new GraphNode();

    b->addDependNode(a);
    c->addDependNode(b);
    c->addDependNode(a);
    d->addDependNode(b);

    graphic->addNode(a);
    graphic->addNode(b);
    graphic->addNode(c);
    graphic->addNode(d);

    cout << graphic->run() << endl;
}

int main() {
    cout << "hello, world" << endl;
    func();

    return 0;
}

