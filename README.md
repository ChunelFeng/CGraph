<p align="left">
  <a href="https://github.com/ChunelFeng/caiss"><img src="https://badgen.net/badge/langs/C++/cyan?list=1" alt="languages"></a>
  <a href="https://github.com/ChunelFeng/caiss"><img src="https://badgen.net/badge/os/MacOS,Linux,Windows/cyan?list=1" alt="os"></a>
</p>

<h1 align="center">
  CGraph说明文档
</h1>

## 一. 简介

本工程实现了一套跨平台图流程计算框架。通过框架图底层调度，实现了相互依赖节点依次顺序执行、非依赖节点同时并发执行的调度功能。

使用者只需继承GraphNode类，实现子类的run方法，并设定本节点依赖的其他节点，即可实现任务的图化执行。


## 二. 使用Demo

#### MyNode1.h
```cpp
#include <iostream>
#include "../../src/GraphCtrl/GraphInclude.h"    // 请根据实际情况，设定引用路径

class MyNode1 : public GraphNode {
public:
    CSTATUS run () override {
        CSTATUS status = STATUS_OK;
        std::cout << "enter node1 run function. sleep for 1 second ... " << std::endl;
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
        CSTATUS status = STATUS_OK;
        std::cout << "enter node2 run function. sleep for 2 second ... " << std::endl;
        this_thread::sleep_for(chrono::milliseconds(2000));
        return status;
    }
};
```

#### demo.cpp
```cpp
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
    GraphNode* a = graphic->registerGraphNode<MyNode1>();
    if (nullptr == a) {
        return;
    }
    GraphNode* b = graphic->registerGraphNode<MyNode2>();
    GraphNode* c = graphic->registerGraphNode<MyNode1>();
    GraphNode* d = graphic->registerGraphNode<MyNode2>();

    status = graphic->addDependNodes(b, {a});       // b节点执行，依赖a节执行
    if (STATUS_OK != status) {
        return;    // 判断返回值是否正确
    }
    status = graphic->addDependNodes(c, {a});       // c节点执行，依赖a节点执行
    status = graphic->addDependNodes(d, {b, c});    // d节点执行，依赖b和c节点

    /* 图信息初始化，准备开始计算 */
    status = graphic->init();

    /* 运行图计算。初始化后，支持多次循环计算 */
    for (int i = 0; i < 3; i++) {
        status = graphic->run();
    }

    /* 图信息逆初始化，准备结束计算 */
    status = graphic->deinit();
    delete graphic;

    return;
}
```

![demo流程执行图](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraphDemo.jpg)
如上图所示，图结构执行的时候，首先执行a节点。a节点执行完毕后，并行执行b和c节点。b和c节点全部执行完毕后，再执行d节点。

------------
#### 附录-1. 版本信息

[2021.05.04 - v1.0.0 - Chunel] 
* 提供图化执行功能，支持非依赖节点并行计算


------------
#### 附录-2. 联系方式: 

* 微信： ChunelFeng
* 邮箱： chunel@foxmail.com
* 源码： https://github.com/ChunelFeng/CGraph
* 论坛： www.chunel.cn
