<p align="left">
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/langs/C++/cyan?list=1" alt="languages"></a>
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/os/MacOS,Linux,Windows/cyan?list=1" alt="os"></a>
  <a href="https://github.com/ChunelFeng/CGraph/stargazers"><img src="https://badgen.net/github/stars/ChunelFeng/CGraph?color=cyan" alt="stars"></a>
  <a href="https://github.com/ChunelFeng/CGraph/network/members"><img src="https://badgen.net/github/forks/ChunelFeng/CGraph?color=cyan" alt="forks"></a>
</p>

<h1 align="center">
  CGraph 说明文档
</h1>

> A cross-platform DAG(Directed Acyclic Graph) pipeline based on C++17 is provided by this repository. </br></br>You can build your own operators simply, and then describe any running schedules for them as you wish, such as dependence, parallelling, loop, condition or aggregation. Some useful plugins and common tools are also given to enhance your project. </br></br>Tutorials and contact information are shown as follows. Please contact us for free if you need more about CGraph.

## 一. 简介

本工程实现了一套跨平台图流程计算框架。通过`GPipeline`(流水线)底层调度，实现了依赖元素依次顺序执行、非依赖元素并发执行的调度功能。

使用者只需继承`GNode`(节点)类，实现子类的run()方法，并根据需要设定依赖关系，即可实现任务的图化执行。

同时，使用者还可以通过设定各种包含多节点信息的`GGroup`(组)，自行控制图的条件判断、循环和并发执行逻辑。

此外，还可以通过添加`GAspect`(切面)的方式，实现以上各种元素功能的横向扩展，或是通过引入各种`GAdapter`(适配器)对单个节点功能进行加强。

![CGraph Skeleton](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Skeleton.jpg)
<br>

## 二. 编译说明
* 本工程支持MacOS、Linux和Windows系统，无任何第三方依赖。使用CLion作为IDE的开发者，打开CMakeLists.txt文件作为工程，即可编译通过

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
#include "../../src/CGraph.h"

class MyNode1 : public CGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        CGraph::CGRAPH_ECHO("[%s], enter MyNode1 run function. Sleep for 1 second ... ", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(1)
        return status;
    }
};
```

#### MyNode2.h
```cpp
#include "../../src/CGraph.h"

class MyNode2 : public CGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        CGraph::CGRAPH_ECHO("[%s], enter MyNode2 run function. Sleep for 2 second ... ", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(2)
        return status;
    }
};
```

#### demo.cpp
```cpp
#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_simple() {
    /* 创建一个流水线，用于设定和执行流图信息 */
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    /* 注册节点，其中MyNode1和MyNode2必须为GNode的子类，否则无法通过编译。
     * MyNode1中run()执行内容为sleep(1s)
     * MyNode2中run()执行内容为sleep(2s) */
    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");    // 将名为nodeA，无执行依赖的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");    // 将名为nodeB，依赖a执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    status += pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");    // 将名为nodeD，依赖{b,c}执行的node信息，注册入pipeline中
    if (!status.isOK()) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定
    }

    /* 执行流图框架 */
    status = pipeline->process();
    GPipelineFactory::destroy(pipeline);
}
```

![CGraph Demo](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Demo.jpg)
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
* 添加对Linux系统的的支持

[2021.06.20 - v1.4.1 - Chunel]
* 提供`condition`（条件）功能
* 添加对Windows系统的支持

[2021.06.24 - v1.5.0 - Chunel]
* 提供`pipeline`工厂创建方法
* 更新`tutorial`内容

[2021.07.07 - v1.5.1 - Chunel]
* 优化线程池功能。实现任务盗取机制

[2021.07.11 - v1.5.2 - Chunel]
* 优化线程池功能。实现线程数量自动调节机制

[2021.07.31 - v1.5.3 - Chunel]
* 优化线程池功能。实现任务批量获取功能，优化任务盗取机制

[2021.08.29 - v1.6.0 - Chunel]
* 提供多`pipeline`功能，优化底层逻辑
* 更新`tutorial`内容

[2021.09.19 - v1.6.1 - Chunel]
* 提供Lru算子、Trie算子和模板节点功能，优化底层逻辑
* 更新`tutorial`内容

[2021.09.29 - v1.7.0 - Chunel]
* 提供`aspect`(切面)功能，用于横向扩展`node`或`group`功能
* 更新`tutorial`内容

[2021.10.07 - v1.7.1 - Chunel]
* 优化`aspect`(切面)实现逻辑，提供切面参数功能，提供批量添加切面功能
* 更新`tutorial`内容

[2021.11.01 - v1.8.0 - Chunel]
* 提供`adapter`(适配器)功能，提供`singleton`适配器功能
* 优化`pipeline`执行逻辑
* 更新`tutorial`内容

[2021.12.18 - v1.8.1 - Chunel]
* 优化了返回值`CStatus`信息

[2022.01.02 - v1.8.2 - Chunel]
* 提供节点执行超时自动退出功能，提供`task group`(任务组)功能
* 提供线程池配置参数设置方法

[2022.01.23 - v1.8.3 - Chunel]
* 提供`function`适配器，实现函数式编程功能
* 提供线程优先级调度功能，提供线程绑定cpu执行功能

------------
#### 附录-2. 推荐阅读

* [纯序员给你介绍图化框架的简单实现——执行逻辑](http://www.chunel.cn/archives/cgraph-run-introduce)
* [纯序员给你介绍图化框架的简单实现——循环逻辑](http://www.chunel.cn/archives/cgraph-loop-introduce)
* [纯序员给你介绍图化框架的简单实现——参数传递](http://www.chunel.cn/archives/cgraph-param-introduce)
* [纯序员给你介绍图化框架的简单实现——条件判断](http://www.chunel.cn/archives/cgraph-condition-introduce)
* [纯序员给你介绍图化框架的简单实现——面向切面](http://www.chunel.cn/archives/cgraph-aspect-introduce)
* [纯序员给你介绍图化框架的简单实现——函数注入](http://www.chunel.cn/archives/cgraph-function-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（一）](http://www.chunel.cn/archives/cgraph-threadpool-1-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（二）](http://www.chunel.cn/archives/cgraph-threadpool-2-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（三）](http://www.chunel.cn/archives/cgraph-threadpool-3-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（四）](http://www.chunel.cn/archives/cgraph-threadpool-4-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（五）](http://www.chunel.cn/archives/cgraph-threadpool-5-introduce)

------------
#### 附录-3. 联系方式

* 微信： ChunelFeng
* 邮箱： chunel@foxmail.com
* 源码： https://github.com/ChunelFeng/CGraph
* 论坛： www.chunel.cn

![CGraph Author](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Author.jpg)
