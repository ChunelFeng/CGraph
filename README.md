<p align="left">
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/langs/C++/cyan?list=1" alt="languages"></a>
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/os/MacOS,Linux,Windows/cyan?list=1" alt="os"></a>
</p>

<h1 align="center">
  CGraph 说明文档
</h1>

## 一. 简介

本工程实现了一套跨平台图流程计算框架。通过框架图底层调度，实现了相互依赖节点依次顺序执行、非依赖节点同时并发执行的调度功能。

使用者只需继承`GNode`类，实现子类的run方法，并设定本节点依赖的其他节点，即可实现任务的图化执行。

同时，使用者还可以通过自行设定各种包含多节点信息的`GGroup`（组），自行控制图的循环和并发执行逻辑。


## 二. 编译说明
* 本工程目前支持MacOS和Linux环境，无任何第三方依赖。使用CLion作为IDE的开发者，直接打开CMakeLists.txt文件作为工程，即可编译。

* Linux环境开发者，在命令行模式下，输入以下指令，即可编译通过
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph/
  $ cmake . -Bbuild
  $ cd build/
  $ make
  ```

## 三. 使用Demo

#### MyNode1.h
```cpp
#include "../../src/GraphCtrl/GraphInclude.h"

class MyNode1 : public GNode {
public:
    CSTATUS run () override {
        CSTATUS status = STATUS_OK;
        CGRAPH_ECHO("[%s], enter MyNode1 run function. Sleep for 1 second ... ", this->getName().c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return status;
    }
};
```

#### MyNode2.h
```cpp
#include "../../src/GraphCtrl/GraphInclude.h"

class MyNode2 : public GNode {
public:
    CSTATUS run () override {
        CSTATUS status = STATUS_OK;
        CGRAPH_ECHO("[%s], enter MyNode2 run function. Sleep for 2 second ... ", this->getName().c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        return status;
    }
};
```

#### demo.cpp
```cpp
#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

void tutorial_simple() {
    /* 创建图化 */
    GPipelinePtr pipeline = new GPipeline();
    CSTATUS status = STATUS_OK;
    GElementPtr a, b, c, d = nullptr;

    /* 注册节点，其中MyNode1和MyNode2必须为GraphNode的子类，否则无法通过编译。
     * MyNode1中run()执行内容为sleep(1s)
     * MyNode2中run()执行内容为sleep(2s) */
    status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");    // 将名为nodeA，无执行依赖的node信息，注册入pipeline中
    if (STATUS_OK != status) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定。本例子中省略
    }
    status = pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB", 3);    // 将名为nodeB，依赖a执行且自循环3次的node信息，注册入pipeline中
    status = pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    status = pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");    // 将名为nodeD，依赖{b,c}执行的node信息，注册入pipeline中

    /* 图信息初始化，准备开始计算 */
    status = pipeline->init();

    /* 运行图计算。初始化后，支持多次循环计算 */
    for (int i = 0; i < 3; i++) {
        status = pipeline->run();
        std::cout << "[CGraph] tutorial_simple, loop : " << i + 1 << ", and run status = " << status << std::endl;
    }

    /* 图信息逆初始化，准备结束计算 */
    status = pipeline->deinit();
    delete pipeline;
}
```

![demo流程执行图](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraphDemo.jpg)
<br>
如上图所示，图结构执行的时候，首先执行`a`节点。`a`节点执行完毕后，并行执行`b`和`c`节点。`b`和`c`节点全部执行完毕后，再执行`d`节点。

------------
#### 附录-1. 版本信息

[2021.05.04 - v1.0.0 - Chunel]
* 提供图化执行功能，支持非依赖节点并行计算

[2021.05.09 - v1.1.0 - Chunel]
* 优化图执行过程中的并发度

[2021.05.18 - v1.1.1 - Chunel]
* 添加节点`name`和`session`信息

[2021.05.23 - v1.2.0 - Chunel]
* 提供单节点循环执行功能

[2021.05.29 - v1.3.0 - Chunel]
* 提供`cluster`（簇）和`region`（区域）划分和循环执行功能
* 提供`tutorial`内容，包含多种使用样例

[2021.06.14 - v1.4.0 - Chunel]
* 提供`param`（参数）传递机制
* 提供`group`（组）功能，多节点模块统一继承自`group`模块
* 添加对Linux系统的支持

------------
#### 附录-2. 推荐阅读

* [纯序员给你介绍图化框架的简单实现——执行逻辑](http://www.chunel.cn/archives/cgraph-run-introduce)
* [纯序员给你介绍图化框架的简单实现——循环逻辑](http://www.chunel.cn/archives/cgraph-loop-introduce)

------------
#### 附录-3. 联系方式

* 微信： ChunelFeng
* 邮箱： chunel@foxmail.com
* 源码： https://github.com/ChunelFeng/CGraph
* 论坛： www.chunel.cn
