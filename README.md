<p align="left">
  <a href="https://github.com/ChunelFeng/caiss"><img src="https://badgen.net/badge/langs/C++/cyan?list=1" alt="languages"></a>
  <a href="https://github.com/ChunelFeng/caiss"><img src="https://badgen.net/badge/os/MacOS,Linux,Windows/cyan?list=1" alt="os"></a>
</p>

<h1 align="center">
  CGraph说明文档
</h1>

## 一. 简介

本工程实现了一套支持linux/mac/windows的跨平台图流程计算框架。通过框架图底层调度，实现了相互依赖节点依次顺序执行、非依赖节点同时并发执行的功能。

使用者只需继承并实现GraphNode类的run方法，设定本节点依赖的其他节点，并且将本节点注册到图框架(graphic类)中，即可实现任务的图化执行。


## 二. 使用Demo

#### MyNode1.h
```cpp
#include <iostream>
#include "../../src/GraphCtrl/GraphInclude.h"    // 请根据实际情况，设定引用路径

class MyNode1 : public GraphNode {
public:
    CSTATUS run () override {
        int status = STATUS_OK;
        std::cout << "enter node1 run functon. sleep for 1 second ... " << std::endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
        return status;
    }
};
```

#### MyNode2.h
```cpp
#include <iostream>
#include "../../src/GraphCtrl/GraphInclude.h"    // 请根据实际情况，设定引用路径

class MyNode2 : public GraphNode {
public:
    CSTATUS run () override {
        int status = STATUS_OK;
        std::cout << "enter node2 run functon. sleep for 2 second ... " << std::endl;
        this_thread::sleep_for(chrono::milliseconds(2000));
        return status;
    }
};
```

#### demo.cpp
```cpp

/* 在正式编程中，请对所有new对象的过程做异常处理
 * 并且对所有接口的返回值做判断 */
#include "MyNode1.h"
#include "MyNode2.h"

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

    status = graphic->init();

    /* 运行图计算 */
    status = graphic->run();

    /* 逆初始化 */
    status = graphic->deinit();
    delete graphic;

    return;
}
```


------------
#### 附录-1. 版本信息

[2021.05.04 - v1.0.0 - Chunel] 
* 提供图化功能
* 支持非依赖节点并行计算

------------
#### 附录-2. 联系方式: 

* 微信： ChunelFeng
* 邮箱： chunel@foxmail.com
* 源码： https://github.com/ChunelFeng/CGraph
* 论坛： www.chunel.cn
