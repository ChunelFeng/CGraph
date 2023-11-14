<p align="left">
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/langs/C++/cyan?list=1" alt="languages"></a>
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/os/MacOS,Linux,Windows/cyan?list=1" alt="os"></a>
  <a href="https://github.com/ChunelFeng/CGraph/stargazers"><img src="https://badgen.net/github/stars/ChunelFeng/CGraph?color=cyan" alt="stars"></a>
  <a href="https://github.com/ChunelFeng/CGraph/network/members"><img src="https://badgen.net/github/forks/ChunelFeng/CGraph?color=cyan" alt="forks"></a>
</p>

[![awesome-cpp](https://badgen.net/badge/icon/awesome-cpp/purple?icon=awesome&label&color)](https://github.com/fffaraz/awesome-cpp)
[![HelloGithub](https://badgen.net/badge/icon/HelloGithub/purple?icon=awesome&label&color)](https://github.com/521xueweihan/HelloGitHub/blob/master/content/HelloGitHub70.md)

中文 | [English Readme](README_en.md)

<h1 align="center">
  CGraph 说明文档
</h1>

<img align="right" src="https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Author.jpg" width="256px">

><b>CGraph</b> is a cross-platform <b>D</b>irected <b>A</b>cyclic <b>G</b>raph framework based on pure C++ without any 3rd-party dependencies.</br></br>
>You, with it, can <b>build your own operators simply, and describe any running schedules</b> as you need, such as dependence, parallelling, aggregation and so on. Some useful tools and plugins are also provide to improve your project.</br></br>
>Tutorials and contact information are show as follows. Please <b>get in touch with us for free</b> if you need more about this repository.

## 一. 简介

`CGraph`中文名为【色丶图】，是一套无任何第三方依赖的跨平台图流程执行框架。通过`GPipeline`(流水线)底层调度，实现了依赖元素依次顺序执行、非依赖元素并发执行的调度功能。

使用者只需继承`GNode`(节点)类，实现子类的run()方法，并根据需要设定依赖关系，即可实现任务的图化执行。还可以通过设定各种包含多节点信息的`GGroup`(组)，自行控制图的条件判断、循环和并发执行逻辑。

项目提供了丰富的`Param`(参数)类型，用于不同应用场景下的数据互通。此外，还可以通过添加`GAspect`(切面)的方式，实现以上各种元素功能的横向扩展；通过引入`GAdapter`(适配器)对单个节点功能进行加强；或者通过添加`GEvent`(信号)，丰富和优化执行逻辑。

![CGraph Skeleton](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Skeleton.jpg)
<br>

详细功能介绍和用法，请参考 **推荐阅读** 中的文章内容。项目相关视频在B站持续更新中，欢迎观看交流和一键三连：<br>
* [【B站视频】CGraph 入门篇](https://www.bilibili.com/video/BV1mk4y1v7XJ) <br>
* [【B站视频】CGraph 功能篇](https://www.bilibili.com/video/BV1CN411w7XB) <br>
* [【B站视频】CGraph 应用篇](https://www.bilibili.com/video/BV1B84y1D7Hs) <br>
* [【B站视频】CGraph 分享篇](https://www.bilibili.com/video/BV1dh4y1i78u) <br>

## 二. 编译说明
* 本工程支持`MacOS`、`Linux`、`Windows`和`Android`系统，无任何第三方依赖。默认使用C++11版本，推荐使用C++17版本，暂不支持C++11以下的版本

* 使用`CLion`(推荐)作为IDE的开发者，打开`CMakeLists.txt`文件作为工程，即可编译通过

* Windows环境中，使用`Visual Studio`(2013版或以上版本)作为IDE的开发者，安装cmake之后，输入以下指令，即可生成`CGraph.sln`文件
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ cmake . -Bbuild    # 在 build 文件夹下，生成对应的 CGraph.sln 文件
  ```

* MacOS环境中，使用`Xcode`作为IDE的开发者，安装cmake之后，输入以下指令，即可生成`CGraph.xcodeproj`文件
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ mkdir build && cd build
  $ cmake .. -G Xcode    # 在 build 文件夹下，生成对应的 CGraph.xcodeproj 文件
  ```

* Linux环境开发者，在命令行模式下，输入以下指令，即可编译通过
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ cmake . -Bbuild
  $ cd build
  $ make -j8
  ```

* 提供online版本的编译调试环境，点击进入页面：[CGraph env online](https://gitpod.io/#/github.com/ChunelFeng/CGraph) ，通过github账号登录。进入后，输入以下指令，即可编译通过，并查看执行结果
  ```shell
  $ sudo apt-get install cmake -y          # 安装cmake
  $ ./CGraph-build.sh                      # 编译CGraph工程，生成的内容在同级/build/文件夹中
  $ ./build/tutorial/T00-HelloCGraph       # 运行第一个实例程序，并且在终端输出 Hello, CGraph.
  ```

## 三. 使用Demo

#### MyNode.h
```cpp
#include "CGraph.h"

class MyNode1 : public CGraph::GNode {
public:
    CStatus run() override {
        printf("[%s], Sleep for 1 second ...\n", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(1)
        return CStatus();
    }
};

class MyNode2 : public CGraph::GNode {
public:
    CStatus run() override {
        printf("[%s], Sleep for 2 second ...\n", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(2)
        return CStatus();
    }
};
```

#### main.cpp
```cpp
#include "MyNode.h"

using namespace CGraph;

int main() {
    /* 创建一个流水线，用于设定和执行流图信息 */
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    /* 注册节点之间的依赖关系 */
    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");
    pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");

    /* 执行流图框架 */
    pipeline->process();
    GPipelineFactory::remove(pipeline);

    return 0;
}
```

![CGraph Demo](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Demo.jpg)
<br>
如上图所示，图结构执行的时候，首先执行`a`节点。`a`节点执行完毕后，并行执行`b`和`c`节点。`b`和`c`节点全部执行完毕后，再执行`d`节点。

## 四. 推荐阅读

* [纯序员给你介绍图化框架的简单实现——执行逻辑](http://www.chunel.cn/archives/cgraph-run-introduce)
* [纯序员给你介绍图化框架的简单实现——循环逻辑](http://www.chunel.cn/archives/cgraph-loop-introduce)
* [纯序员给你介绍图化框架的简单实现——参数传递](http://www.chunel.cn/archives/cgraph-param-introduce)
* [纯序员给你介绍图化框架的简单实现——条件判断](http://www.chunel.cn/archives/cgraph-condition-introduce)
* [纯序员给你介绍图化框架的简单实现——面向切面](http://www.chunel.cn/archives/cgraph-aspect-introduce)
* [纯序员给你介绍图化框架的简单实现——函数注入](http://www.chunel.cn/archives/cgraph-function-introduce)
* [纯序员给你介绍图化框架的简单实现——消息机制](http://www.chunel.cn/archives/cgraph-message-introduce)
* [纯序员给你介绍图化框架的简单实现——事件触发](http://www.chunel.cn/archives/cgraph-event-introduce)
* [纯序员给你介绍图化框架的简单实现——超时机制](http://www.chunel.cn/archives/cgraph-timeout-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（一）](http://www.chunel.cn/archives/cgraph-threadpool-1-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（二）](http://www.chunel.cn/archives/cgraph-threadpool-2-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（三）](http://www.chunel.cn/archives/cgraph-threadpool-3-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（四）](http://www.chunel.cn/archives/cgraph-threadpool-4-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（五）](http://www.chunel.cn/archives/cgraph-threadpool-5-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（六）](http://www.chunel.cn/archives/cgraph-threadpool-6-introduce)
* [纯序员给你介绍图化框架的简单实现——性能优化（一）](http://www.chunel.cn/archives/cgraph-performance-1)
* [纯序员给你介绍图化框架的简单实现——距离计算](http://www.chunel.cn/archives/cgraph-distance-introduce)
  <br><br>
* [CGraph 主打歌——《听码农的话》](http://www.chunel.cn/archives/listen-to-coder)
* [聊聊我写CGraph的这一年](http://www.chunel.cn/archives/cgraph-anniversary-introduce)
* [从零开始主导一款收录于awesome-cpp的项目，是一种怎样的体验?](http://www.chunel.cn/archives/cgraph-awesome-cpp)
* [炸裂！CGraph性能全面超越taskflow之后，作者却说他更想...](http://www.chunel.cn/archives/cgraph-compare-taskflow-v1)
* [以图优图：CGraph中计算dag最大并发度思路总结](http://www.chunel.cn/archives/cgraph-max-para-size)
* [一文带你了解练习时长两年半的CGraph](http://www.chunel.cn/archives/cgraph-kunanniversary-introduce)
  <br><br>

## 五. 关联项目

* [GraphANNS](https://github.com/whenever5225/GraphANNS) : Graph-based Approximate Nearest Neighbor Search Working off CGraph
* [CThreadPool](https://github.com/ChunelFeng/CThreadPool) : 一个简单好用、功能强大、性能优异、跨平台的C++线程池
* [taskflow](https://github.com/taskflow/taskflow) : A General-purpose Parallel and Heterogeneous Task Programming System
* [awesome-cpp](https://github.com/fffaraz/awesome-cpp) : A curated list of awesome C++ (or C) frameworks, libraries, resources, and shiny things. Inspired by awesome-... stuff.
* [awesome-workflow-engines](https://github.com/meirwah/awesome-workflow-engines) : A curated list of awesome open source workflow engines

------------
<details>
<summary><b>附录-1. 版本信息</b></summary>

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
* 提供`Lru`算子、`Trie`算子和模板节点功能，优化底层逻辑
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
* 更新`tutorial`内容

[2022.01.31 - v1.8.4 - Chunel]
* 提供`node`(节点)异步执行的功能

[2022.02.03 - v1.8.5 - Chunel]
* 提供`daemon`(守护)功能，用于定时执行非流图中任务
* 更新`tutorial`内容

[2022.04.03 - v1.8.6 - Chunel]
* 提供`DistanceCalculator`算子，用于实现任意数据类型、任意距离类型的计算
* 更新`tutorial`内容

[2022.04.05 - v2.0.0 - Chunel]
* 提供`domain`(领域)功能，提供`Ann`领域抽象模型，开始支持个别专业方向
* 提供hold执行机制
* 更新`tutorial`内容

[2022.05.01 - v2.0.1 - Chunel]
* 优化`pipeline`注册机制，支持init方法自定义顺序执行
* 提供一键编译脚本

[2022.05.29 - v2.1.0 - Chunel]
* 提供`element`参数写入方法
* 提供针对C++14版本的支持
* 更新`tutorial`内容

[2022.10.03 - v2.1.1 - Chunel]
* 提供线程池中的任务优先级机制
* 优化`group`执行逻辑

[2022.11.03 - v2.2.0 - Chunel]
* 提供`message`(消息)功能，主要用于完成不同`pipeline`之间的数据传递
* 更新`tutorial`内容

[2022.12.24 - v2.2.1 - Chunel]
* 提供`TemplateNode`(模板节点)功能，用于优化参数传参方式
* 更新`tutorial`内容

[2022.12.25 - v2.2.2 - [yeshenyong](https://github.com/yeshenyong)]
* 优化图执行逻辑

[2022.12.30 - v2.2.3 - Chunel]
* 提供`message`发布订阅功能
* 提供执行引擎切换功能

[2023.01.21 - v2.3.0 - Chunel]
* 提供`event`(事件)功能
* 提供`CGraph Intro.xmind`文件，通过脑图的方式，介绍了CGraph的整体逻辑

[2023.01.25 - v2.3.1 - Chunel]
* 提供针对C++11版本的支持。感谢 [MirrorYuChen](https://github.com/MirrorYuChen) 提供相关解决方案

[2023.02.10 - v2.3.2 - Chunel]
* 优化调度策略，提供调度参数配置接口
* 提供英文版本readme.md

[2023.02.12 - v2.3.3 - [yeshenyong](https://github.com/yeshenyong), Chunel]
* 提供graphviz可视化图展示功能
* 提供参数链路追踪功能

[2023.02.22 - v2.3.4 - Chunel]
* 优化Windows系统下调度机制
* 优化`param`机制和`event`机制

[2023.03.25 - v2.4.0 - [woodx](https://github.com/woodx9), Chunel]
* 提供可运行的docker环境，和构建docker环境的dockerfile文件
* 提供`pipeline`调度资源管控机制
* 优化调度性能

[2023.05.05 - v2.4.1 - Chunel]
* 提供线程绑定执行功能
* 提供`pipeline`最大并发度获取方法。感谢 [Hanano-Yuuki](https://github.com/Hanano-Yuuki) 提供相关解决方案
* 提供`pipeline`异步执行功能和执行时退出功能

[2023.06.17 - v2.4.2 - Chunel]
* 提供`MultiCondition`(多条件)功能
* 提供`pipeline`暂停执行和恢复执行功能

[2023.07.12 - v2.4.3 - Chunel]
* 优化`CStatus`功能，添加了异常定位信息

[2023.09.05 - v2.5.0 - Chunel]
* 提供perf功能，用于做`pipeline`的性能分析
* 提供`element`的超时机制
* 提供`some`(部分)功能，优化`pipeline`的异步执行方式

[2023.09.15 - v2.5.1 - Chunel]
* 提供`fence`(栅栏)功能
* 提供`coordinator`(协调)功能

[2023.11.06 - v2.5.2 - Chunel]
* 优化`message`(消息)功能，可以设定写入阻塞时的处理方式，减少内存copy次数
* 添加`example`相关内容，针对不同行业，提供一些简单实现
* 优化调度性能

[2023.11.15 - v2.5.3 - Chunel]
* 提供`proto`定义文件
* 添加`mutable`(异变)功能，提供依赖关系注册语法糖

</details>

------------
<details>
<summary><b>附录-2. 感谢</b></summary>

* 感谢 [Doocs 微信公众号](https://mp.weixin.qq.com/mp/appmsgalbum?__biz=MzIxNjA5ODQ0OQ==&action=getalbum&album_id=1989460124624551937&scene=173&from_msgid=2654703194&from_itemidx=1&count=3&nolastread=1#wechat_redirect) 刊登相关介绍文档，欢迎加入 [Doocs 开源社区](https://github.com/doocs)

* 感谢《HelloGithub》期刊介绍和推荐：[HelloGithub 第70期](https://github.com/521xueweihan/HelloGitHub/blob/master/content/HelloGitHub70.md)

<p align="center"><img src="https://github.com/ChunelFeng/CGraph/blob/main/doc/image/HelloGithub%20Logo.gif"/></p>

* 感谢《Github中文排行榜》介绍和推荐：[Github中文排行榜 增速榜-C++分类](https://github.com/GrowingGit/GitHub-Chinese-Top-Charts/blob/master/content/charts/growth/software/CPP.md)

<p align="center"><img src="https://github.com/ChunelFeng/CGraph/blob/main/doc/image/GrowingGit%20Logo.png"/></p>

* Thanks to the recommendation from [awesome-cpp](https://github.com/fffaraz/awesome-cpp), we all know, it is the most authoritative recommendation list for cpp project in the world
* Thanks to the recommendation from `Taskflow Group`: [awesome-parallel-computing](https://github.com/taskflow/awesome-parallel-computing), and we always treat [taskflow](https://github.com/taskflow/taskflow) as a role model
* Thanks to the recommendation from [awesome-workflow-engines](https://github.com/meirwah/awesome-workflow-engines)
* 感谢各位开发者 [CONTRIBUTORS](https://github.com/ChunelFeng/CGraph/blob/main/CONTRIBUTORS.md) 为项目做出的贡献
* 感谢所有为`CGraph`项目提出的意见和建议的朋友，在此不一一提及。随时欢迎大家加入，一起共建

</details>

------------
<details>
<summary><b>附录-3. 联系方式</b></summary>

* 微信： ChunelFeng (欢迎扫描上方二维码，和项目主要作者成为好友。请简单备注个人信息^_^)
* 邮箱： chunel@foxmail.com
* 源码： https://github.com/ChunelFeng/CGraph
* 论坛： www.chunel.cn

</details>
