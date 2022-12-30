<p align="left">
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/langs/C++/cyan?list=1" alt="languages"></a>
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/os/MacOS,Linux,Windows/cyan?list=1" alt="os"></a>
  <a href="https://github.com/ChunelFeng/CGraph/stargazers"><img src="https://badgen.net/github/stars/ChunelFeng/CGraph?color=cyan" alt="stars"></a>
  <a href="https://github.com/ChunelFeng/CGraph/network/members"><img src="https://badgen.net/github/forks/ChunelFeng/CGraph?color=cyan" alt="forks"></a>
</p>

[![awesome-cpp](https://badgen.net/badge/icon/awesome-cpp/purple?icon=awesome&label&color)](https://github.com/fffaraz/awesome-cpp)
[![HelloGithub](https://badgen.net/badge/icon/HelloGithub/purple?icon=awesome&label&color)](https://github.com/521xueweihan/HelloGitHub/blob/master/content/HelloGitHub70.md)

<h1 align="center">
  CGraph 说明文档
</h1>

><b>CGraph</b> is a cross-platform <b>D</b>irected <b>A</b>cyclic <b>G</b>raph framework based on pure C++ without any 3rd-party.</br></br>
>You, with it, can build your own operators simply, and describe any running schedules as you need, such as dependence, parallelling, aggregation and so on. Some useful tools and plugins are also provide to improve your project.</br></br>
>Tutorials and contact information are show as follows. Please get in touch with us for free if you need more about this repository.

## 一. 简介

`CGraph`中文名为【色丶图】，是一套无任何第三方依赖的跨平台图流程执行框架。通过`GPipeline`(流水线)底层调度，实现了依赖元素依次顺序执行、非依赖元素并发执行的调度功能。

使用者只需继承`GNode`(节点)类，实现子类的run()方法，并根据需要设定依赖关系，即可实现任务的图化执行。还可以通过设定各种包含多节点信息的`GGroup`(组)，自行控制图的条件判断、循环和并发执行逻辑。

项目提供了丰富的参数类型，用于不同应用场景下的数据互通。此外，还可以通过添加`GAspect`(切面)的方式，实现以上各种元素功能的横向扩展，或是通过引入`GAdapter`(适配器)对单个节点功能进行加强。

![CGraph Skeleton](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Skeleton.jpg)
<br>


## 二. 编译说明
* 本工程支持MacOS、Linux和Windows系统，无任何第三方依赖。推荐使用C++17（默认）或以上版本，向下兼容C++14版本，不支持C++11或以下版本

* 使用`CLion`作为IDE的开发者，或使用`Visual Studio 17`(或以上版本)作为IDE的开发者，打开`CMakeLists.txt`文件作为工程，即可编译通过

* 使用`Visual Studio 2015`作为IDE的开发者，修改`CMakeLists.txt`文件中的`set(CMAKE_CXX_STANDARD 17)`为`set(CMAKE_CXX_STANDARD 14)`，然后打开`CMakeLists.txt`文件作为工程，即可编译通过

* Linux环境开发者，在命令行模式下，输入以下指令，即可编译通过
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ cmake . -Bbuild
  $ cd build
  $ make -j8
  ```

* 提供基于`Ubuntu 20.04`的Docker镜像。输入以下指令，即可获取并进入
  ```shell
  $ docker pull chunelfeng/cenv                         # 获取docker镜像
  $ docker run -it --name CGraphEnv chunelfeng/cenv     # 开启docker容器，并进入
  ```

* 提供online版本的编译调试环境，点击进入页面：[CGraph env online](https://gitpod.io/#/github.com/ChunelFeng/CGraph) ，输入以下指令，即可编译通过，并查看执行结果
  ```shell
  $ ./CGraph-build.sh          # 编译CGraph工程，生成的内容在同级/build/文件夹中
  $ ./build/T00-HelloCGraph    # 运行第一个实例程序，并且在终端输出 Hello, CGraph.
  ```

## 三. 使用Demo

#### MyNode.h
```cpp
#include "../src/CGraph.h"

class MyNode1 : public CGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        CGraph::CGRAPH_ECHO("[%s], enter MyNode1 run function. Sleep for 1 second ... ", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(1)
        return status;
    }
};


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

#### main.cpp
```cpp
#include "MyNode.h"

using namespace CGraph;

int main() {
    /* 创建一个流水线，用于设定和执行流图信息 */
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    /* 注册节点，其中MyNode1和MyNode2必须为GNode的子类，否则无法通过编译 */
    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");    // 将名为nodeA，无执行依赖的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");    // 将名为nodeB，依赖a执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    status += pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");    // 将名为nodeD，依赖{b,c}执行的node信息，注册入pipeline中
    if (!status.isOK()) {
        return;    // 对以上所有CGraph接口的返回值做判定
    }

    /* 执行流图框架 */
    status = pipeline->process();
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
* [纯序员给你介绍图化框架的简单实现——线程池优化（一）](http://www.chunel.cn/archives/cgraph-threadpool-1-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（二）](http://www.chunel.cn/archives/cgraph-threadpool-2-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（三）](http://www.chunel.cn/archives/cgraph-threadpool-3-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（四）](http://www.chunel.cn/archives/cgraph-threadpool-4-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（五）](http://www.chunel.cn/archives/cgraph-threadpool-5-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（六）](http://www.chunel.cn/archives/cgraph-threadpool-6-introduce)
* [纯序员给你介绍图化框架的简单实现——距离计算](http://www.chunel.cn/archives/cgraph-distance-introduce)
  <br><br>
* [CGraph 主打歌——《听码农的话》](http://www.chunel.cn/archives/listen-to-coder)
* [聊聊我写CGraph的这一年](http://www.chunel.cn/archives/cgraph-anniversary-introduce)
  <br><br>
* [【B站视频】CGraph 快速引入的方法介绍](https://www.bilibili.com/video/BV1gY4y1x7JT?spm_id_from=333.337)

## 五. 关联项目

* [GraphANNS](https://github.com/whenever5225/GraphANNS) : Graph-based Approximate Nearest Neighbor Search Working off CGraph
* [CThreadPool](https://github.com/ChunelFeng/CThreadPool) : 一个简单好用、功能强大、性能优异、跨平台的C++线程池

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
* 提供hold执行机制，支持根据运行时条件，判断是否需要重新执行当前内容，直到满足条件为止
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
* 感谢 [@yangyuxiang77](https://github.com/yangyuxiang77) [@logerrors](https://github.com/logerrors) [@whenever5225](https://github.com/whenever5225) [@May-Yaha](https://github.com/May-Yaha) [@Codesire-Deng](https://github.com/Codesire-Deng) [@yeshenyong](https://github.com/yeshenyong) 等朋友（以贡献时间先后为顺序）为项目做出的贡献
* 感谢所有为`CGraph`项目提出的意见和建议的朋友，在此不一一提及。随时欢迎大家加入，一起共建

</details>

------------
<details>
<summary><b>附录-3. 联系方式</b></summary>

* 微信： ChunelFeng
* 邮箱： chunel@foxmail.com
* 源码： https://github.com/ChunelFeng/CGraph
* 论坛： www.chunel.cn

![CGraph Author](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Author.jpg)

</details>
