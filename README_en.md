<p align="left">
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/langs/C++,Python/cyan?list=1" alt="languages"></a>
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/os/MacOS,Linux,Windows/cyan?list=1" alt="os"></a>
  <a href="https://github.com/ChunelFeng/CGraph/stargazers"><img src="https://badgen.net/github/stars/ChunelFeng/CGraph?color=cyan" alt="stars"></a>
  <a href="https://github.com/ChunelFeng/CGraph/network/members"><img src="https://badgen.net/github/forks/ChunelFeng/CGraph?color=cyan" alt="forks"></a>
  <a href="https://badge.fury.io/py/pycgraph"><img src="https://badge.fury.io/py/pycgraph.svg" alt="pypi"></a>
  <a href="https://pepy.tech/projects/pycgraph"><img src="https://static.pepy.tech/personalized-badge/pycgraph?period=total&units=INTERNATIONAL_SYSTEM&left_color=GRAY&right_color=GREEN&left_text=pypi+downloads" alt="PyPI Downloads"></a>
  <a href="https://www.codefactor.io/repository/github/chunelfeng/cgraph/overview/main"><img src="https://www.codefactor.io/repository/github/chunelfeng/cgraph/badge/main" alt="CodeFactor" /></a>
</p>

[![awesome-cpp](https://badgen.net/badge/icon/awesome-cpp/purple?icon=awesome&label&color)](https://github.com/fffaraz/awesome-cpp)
[![HelloGithub](https://badgen.net/badge/icon/HelloGithub/purple?icon=awesome&label&color)](https://github.com/521xueweihan/HelloGitHub/blob/master/content/HelloGitHub70.md)

[Chinese Readme](README.md) | English Readme | [deepwiki](https://deepwiki.com/ChunelFeng/CGraph)

<h1 align="center"> CGraph Readme </h1>

<img align="right" src="https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Author.jpg" width="256px">

><b>CGraph</b> is a cross-platform <b>D</b>irected <b>A</b>cyclic <b>G</b>raph framework based on pure C++ without any 3rd-party dependencies.</br></br>
>You, with it, can <b>build your own operators simply, and describe any running schedules</b> as you need, such as dependence, parallelling, aggregation, conditional and so on. <b>Python APIs</b> are also supported to build your pipeline.</br></br>
>Tutorials and contact information are shown as follows. Please <b>get in touch with us for free</b> if you need more about this repository.

## 1. Introduction

The Chinese name of `CGraph` is "Se丶Tu". It is a cross-platform graph flow execution framework without any third-party dependencies. Through the underlying scheduling of `GPipeline`, it provides `eDAG` scheduling features including sequential execution of dependent elements, concurrent execution of independent elements, pause, resume, and timeout settings.

Users only need to inherit from the `GNode` class, implement the `run()` method in the subclass, and set dependencies as needed to execute tasks as a graph or as a pipeline. Users can also configure different `GGroup`s containing multiple nodes to control conditional judgment, loop, and concurrent execution logic by themselves.

![CGraph Skeleton](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Skeleton.jpg)
<br>

This project is written with only the C++11 standard library and has no third-party dependencies. It is compatible with `MacOS`, `Linux`, `Windows`, and `Android`. It supports local compilation and secondary development, and also provides a `Python` version: `pycgraph`. For compilation and installation, please refer to [CGraph Compile Guide](https://github.com/ChunelFeng/CGraph/blob/main/COMPILE.md ) <br>

For detailed feature introductions and usage, please refer to the articles on [Chunel's Blog](http://www.chunel.cn/). Related videos are continuously updated on Bilibili. Welcome to watch and discuss:<br>
* [Bilibili Video: CGraph Getting Started](https://www.bilibili.com/video/BV1mk4y1v7XJ) <br>
* [Bilibili Video: CGraph Features](https://www.bilibili.com/cheese/play/ss22264) <br>
  * A complete introduction to all terms and feature modules in the CGraph project
  * Detailed explanations of each feature's usage scenarios, usage methods, and solved problems with real coding processes
  * Suitable for users who want to fully understand CGraph features and get started quickly
  * Suitable for users interested in multithreaded programming
* [Bilibili Video: CGraph Applications](https://www.bilibili.com/video/BV1B84y1D7Hs) <br>
* [Bilibili Video: CGraph Sharing](https://www.bilibili.com/video/BV1ofLdz5EzX) <br>

----

## 2. Getting Started Demo
> <b>C++ Version</b>
```cpp
#include "CGraph.h"

using namespace CGraph;

class MyNode1 : public GNode {
public:
    CStatus run() override {
        printf("[%s], sleep for 1 second ...\n", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(1)
        return CStatus();
    }
};

class MyNode2 : public GNode {
public:
    CStatus run() override {
        printf("[%s], sleep for 2 second ...\n", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(2)
        return CStatus();
    }
};


int main() {
    /* Create a pipeline for configuring and executing graph flow information */
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    /* Register dependencies between nodes */
    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");
    pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");

    /* Execute the graph flow framework */
    pipeline->process();

    /* Clear all resources in the pipeline */
    GPipelineFactory::remove(pipeline);

    return 0;
}
```

![CGraph Demo](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Demo.jpg)
<br>
As shown above, when the graph structure is executed, node `a` runs first. After node `a` finishes, nodes `b` and `c` run in parallel. After both `b` and `c` finish, node `d` runs.

> <b>Python Version</b>

```python
import time
from datetime import datetime

from pycgraph import GNode, GPipeline, CStatus


class MyNode1(GNode):
    def run(self):
        print("[{0}] {1}, enter MyNode1 run function. Sleep for 1 second ... ".format(datetime.now(), self.getName()))
        time.sleep(1)
        return CStatus()

class MyNode2(GNode):
    def run(self):
        print("[{0}] {1}, enter MyNode2 run function. Sleep for 2 second ... ".format(datetime.now(), self.getName()))
        time.sleep(2)
        return CStatus()


if __name__ == '__main__':
    pipeline = GPipeline()
    a, b, c, d = MyNode1(), MyNode2(), MyNode1(), MyNode2()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {b, c}, "nodeD")

    pipeline.process()
```

> <b>Other Versions</b>

* [CsCGraph](https://github.com/ChunelFeng/CsCGraph) : A CSharp native, CGraph-API-liked DAG project
* [JaCGraph](https://github.com/ChunelFeng/JaCGraph) : A Java native, CGraph-API-liked DAG project
* [GoCGraph](https://github.com/AsunaU2/GoCGraph) : A Go native, CGraph-API-liked DAG project
* [CGraph-lite](https://github.com/ChunelFeng/CGraph-lite) : A one-header-only, CGraph-API-liked DAG project, lite version by C++

----

## 3. Recommended Reading

* [A simple implementation of a graph framework - execution logic](http://www.chunel.cn/archives/cgraph-run-introduce)
* [A simple implementation of a graph framework - loop logic](http://www.chunel.cn/archives/cgraph-loop-introduce)
* [A simple implementation of a graph framework - parameter passing](http://www.chunel.cn/archives/cgraph-param-introduce)
* [A simple implementation of a graph framework - conditional judgment](http://www.chunel.cn/archives/cgraph-condition-introduce)
* [A simple implementation of a graph framework - aspect-oriented extension](http://www.chunel.cn/archives/cgraph-aspect-introduce)
* [A simple implementation of a graph framework - function injection](http://www.chunel.cn/archives/cgraph-function-introduce)
* [A simple implementation of a graph framework - message mechanism](http://www.chunel.cn/archives/cgraph-message-introduce)
* [A simple implementation of a graph framework - event triggering](http://www.chunel.cn/archives/cgraph-event-introduce)
* [A simple implementation of a graph framework - timeout mechanism](http://www.chunel.cn/archives/cgraph-timeout-introduce)
* [A simple implementation of a graph framework - thread pool optimization (1)](http://www.chunel.cn/archives/cgraph-threadpool-1-introduce)
* [A simple implementation of a graph framework - thread pool optimization (2)](http://www.chunel.cn/archives/cgraph-threadpool-2-introduce)
* [A simple implementation of a graph framework - thread pool optimization (3)](http://www.chunel.cn/archives/cgraph-threadpool-3-introduce)
* [A simple implementation of a graph framework - thread pool optimization (4)](http://www.chunel.cn/archives/cgraph-threadpool-4-introduce)
* [A simple implementation of a graph framework - thread pool optimization (5)](http://www.chunel.cn/archives/cgraph-threadpool-5-introduce)
* [A simple implementation of a graph framework - thread pool optimization (6)](http://www.chunel.cn/archives/cgraph-threadpool-6-introduce)
* [A simple implementation of a graph framework - performance optimization (1)](http://www.chunel.cn/archives/cgraph-performance-1)
* [A simple implementation of a graph framework - performance optimization (2)](http://www.chunel.cn/archives/cgraph-performance-2)
* [A simple implementation of a graph framework - distance calculation](http://www.chunel.cn/archives/cgraph-distance-introduce)
  <br><br>
* [CGraph theme song - Listen to the Coder](http://www.chunel.cn/archives/listen-to-coder)
* [Talking about the year I spent writing CGraph](http://www.chunel.cn/archives/cgraph-anniversary-introduce)
* [What is it like to lead an awesome-cpp project from scratch?](http://www.chunel.cn/archives/cgraph-awesome-cpp)
* [Explosive! After CGraph performance fully surpasses taskflow, the author says he wants more...](http://www.chunel.cn/archives/cgraph-compare-taskflow-v1)
* [Optimizing graphs with graphs: ideas for calculating the maximum DAG parallelism in CGraph](http://www.chunel.cn/archives/cgraph-max-para-size)
* [One article to understand CGraph after two and a half years of practice](http://www.chunel.cn/archives/cgraph-kunanniversary-introduce)
* [The CGraph author wants to know whether you need an eDAG scheduling framework](http://www.chunel.cn/archives/cgraph-extended-dag)
* [Reducing edges and improving efficiency: summary of redundant edge pruning in CGraph](http://www.chunel.cn/archives/cgraph-remove-redundancy-link)
* [Latest coding-world feel-good story: reborn as someone writing CGraph abroad (Python version)](http://www.chunel.cn/archives/cgraph-pycgraph-v1)
* [Building the CGraph I once did not dare to imagine](http://www.chunel.cn/archives/cgraph-unbelievable-2025)
  <br>

----

## 4. Related Projects

* [GraphANNS](https://github.com/whenever5225/GraphANNS) : Graph-based Approximate Nearest Neighbor Search Working off CGraph
* [CThreadPool](https://github.com/ChunelFeng/CThreadPool) : A simple, easy-to-use, powerful, high-performance, cross-platform C++ thread pool
* [PyCGraph-example](https://github.com/ChunelFeng/PyCGraph-example) : A useful list of how cool to use PyCGraph
* [awesome-cpp](https://github.com/fffaraz/awesome-cpp) : A curated list of awesome C++ (or C) frameworks, libraries, resources, and shiny things. Inspired by awesome-... stuff.
* [awesome-workflow-engines](https://github.com/meirwah/awesome-workflow-engines) : A curated list of awesome open source workflow engines
* [taskflow](https://github.com/taskflow/taskflow) : A General-purpose Parallel and Heterogeneous Task Programming System
  * [Bilibili Video: CGraph and taskflow performance comparison test](https://www.bilibili.com/video/BV1gwWAekEAy/?spm_id_from=333.337.search-card.all.click&vd_source=2c7baed805c6cb33d630d5d4546cf0be) <br>
* [torchpipe](https://github.com/torchpipe/torchpipe) : Serving Inside Pytorch
  * [Bilibili Video: open-source project torchpipe - online high-concurrency experience and practice for AI engines](https://www.bilibili.com/video/BV1Zm411X7k1/)
* [nndeploy](https://github.com/DeployAI/nndeploy) : Easy-to-use, high-performance, multi-platform inference deployment framework
  * [Bilibili Video Collection: nndeploy AI inference framework complete tutorial](https://space.bilibili.com/435543077/lists/5565358?type=season)
* [KuiperInfer](https://github.com/zjhellofss/KuiperInfer) : A step-by-step guide to building a high-performance deep learning inference library, supporting inference for large models such as llama2, Unet, Yolov5, and Resnet. Implement a high-performance deep learning inference library step by step
  * [Bilibili Video: KuiperInfer inference framework - a teaching-oriented inference framework](https://www.bilibili.com/video/BV1t2421K7HN)
* [OGraph](https://github.com/symphony09/ograph) : A simple way to build a pipeline with Go.
  * [Bilibili Video: Alibaba Cloud expert sharing - OGraph, notes on Go-based graph flow scheduling](https://www.bilibili.com/video/BV19RHce6Evo)
* [incubator-hugegraph-ai](https://github.com/apache/incubator-hugegraph-ai) : The integration of HugeGraph with AI/LLM & GraphRAG
  * [Agentic GraphRAG: modular architecture practice](https://hugegraph.apache.org/cn/blog/2025/10/29/agentic-graphrag/)
* [pybind11](https://github.com/pybind/pybind11) : Seamless operability between C++11 and Python
  * The Python API binding feature of this project is implemented with pybind11
  * [Bilibili Video: pybind11 practical implementation - how PyCGraph was built](https://www.bilibili.com/video/BV1ofLdz5EzX) | [Feishu document link](https://ilyryn6yei.feishu.cn/docx/YDfHdU9mfoTlnqxaTkscRob6nGe)


[![Star History Chart](https://star-history.dera.page/svg?repos=ChunelFeng/CGraph&type=Date)](https://star-history.dera.page/#ChunelFeng/CGraph&Date)

------------
<details>
<summary><b>Appendix 1. Version Information</b></summary>

[2021.05.04 - v1.0.0 - Chunel]
* Provide graph execution and support parallel computation of independent nodes

[2021.05.09 - v1.1.0 - Chunel]
* Optimize concurrency during graph execution

[2021.05.18 - v1.1.1 - Chunel]
* Add node `name` and `session` information

[2021.05.23 - v1.2.0 - Chunel]
* Provide single-node loop execution

[2021.05.29 - v1.3.0 - Chunel]
* Provide `cluster` and `region` division and loop execution
* Provide `tutorial` content with multiple usage examples

[2021.06.14 - v1.4.0 - Chunel]
* Provide the `param` passing mechanism
* Provide the `group` feature, with multi-node modules uniformly inheriting from the `group` module
* Add support for Linux

[2021.06.20 - v1.4.1 - Chunel]
* Provide the `condition` feature
* Add support for Windows

[2021.06.24 - v1.5.0 - Chunel]
* Provide the `pipeline` factory creation method
* Update `tutorial` content

[2021.07.07 - v1.5.1 - Chunel]
* Optimize the thread pool and implement task stealing

[2021.07.11 - v1.5.2 - Chunel]
* Optimize the thread pool and implement automatic thread-count adjustment

[2021.07.31 - v1.5.3 - Chunel]
* Optimize the thread pool, implement batch task retrieval, and optimize task stealing

[2021.08.29 - v1.6.0 - Chunel]
* Provide multi-`pipeline` support and optimize underlying logic
* Update `tutorial` content

[2021.09.19 - v1.6.1 - Chunel]
* Provide the `Lru` operator, `Trie` operator, and template node, and optimize underlying logic
* Update `tutorial` content

[2021.09.29 - v1.7.0 - Chunel]
* Provide the `aspect` feature for horizontally extending `node` or `group` capabilities
* Update `tutorial` content

[2021.10.07 - v1.7.1 - Chunel]
* Optimize `aspect` implementation logic, provide aspect parameters, and support batch aspect addition
* Update `tutorial` content

[2021.11.01 - v1.8.0 - Chunel]
* Provide the `adapter` feature and the `singleton` adapter feature
* Optimize `pipeline` execution logic
* Update `tutorial` content

[2021.12.18 - v1.8.1 - Chunel]
* Optimize return-value `CStatus` information

[2022.01.02 - v1.8.2 - Chunel]
* Provide automatic exit on node execution timeout and the `task group` feature
* Provide thread pool configuration parameter setters

[2022.01.23 - v1.8.3 - Chunel]
* Provide the `function` adapter to implement functional programming
* Provide thread-priority scheduling and CPU-binding execution
* Update `tutorial` content

[2022.01.31 - v1.8.4 - Chunel]
* Provide asynchronous execution for `node`

[2022.02.03 - v1.8.5 - Chunel]
* Provide the `daemon` feature for periodically executing tasks outside the graph flow
* Update `tutorial` content

[2022.04.03 - v1.8.6 - Chunel]
* Provide the `DistanceCalculator` operator for computing arbitrary data types and arbitrary distance types
* Update `tutorial` content

[2022.04.05 - v2.0.0 - Chunel]
* Provide the `domain` feature and the `Ann` domain abstraction model, beginning support for specific professional domains
* Provide the hold execution mechanism
* Update `tutorial` content

[2022.05.01 - v2.0.1 - Chunel]
* Optimize the `pipeline` registration mechanism and support custom sequential execution of the init method
* Provide a one-click build script

[2022.05.29 - v2.1.0 - Chunel]
* Provide an `element` parameter writing method
* Provide support for C++14
* Update `tutorial` content

[2022.10.03 - v2.1.1 - Chunel]
* Provide a task-priority mechanism in the thread pool
* Optimize `group` execution logic

[2022.11.03 - v2.2.0 - Chunel]
* Provide the `message` feature, mainly for data transfer between different `pipeline`s
* Update `tutorial` content

[2022.12.24 - v2.2.1 - Chunel]
* Provide the `TemplateNode` feature to optimize parameter passing
* Update `tutorial` content

[2022.12.25 - v2.2.2 - [yeshenyong](https://github.com/yeshenyong)]
* Optimize graph execution logic

[2022.12.30 - v2.2.3 - Chunel]
* Provide `message` publish-subscribe support
* Provide execution engine switching

[2023.01.21 - v2.3.0 - Chunel]
* Provide the `event` feature
* Provide the `CGraph Intro.xmind` file to introduce the overall CGraph logic through a mind map

[2023.01.25 - v2.3.1 - Chunel]
* Provide support for C++11. Thanks to [MirrorYuChen](https://github.com/MirrorYuChen) for the related solution

[2023.02.10 - v2.3.2 - Chunel]
* Optimize the scheduling strategy and provide a scheduling parameter configuration interface
* Provide the English version of readme.md

[2023.02.12 - v2.3.3 - [yeshenyong](https://github.com/yeshenyong), Chunel]
* Provide graphviz visualization for graph display
* Provide parameter chain tracing

[2023.02.22 - v2.3.4 - Chunel]
* Optimize the scheduling mechanism on Windows
* Optimize the `param` mechanism and the `event` mechanism

[2023.03.25 - v2.4.0 - [woodx](https://github.com/woodx9), Chunel]
* Provide a runnable Docker environment and the Dockerfile for building it
* Provide the `pipeline` scheduling resource control mechanism
* Optimize scheduling performance

[2023.05.05 - v2.4.1 - Chunel]
* Provide thread-binding execution
* Provide a method to get the maximum parallelism of a `pipeline`. Thanks to [Hanano-Yuuki](https://github.com/Hanano-Yuuki) for the related solution
* Provide asynchronous `pipeline` execution and exit during execution

[2023.06.17 - v2.4.2 - Chunel]
* Provide the `MultiCondition` feature
* Provide pause and resume execution for `pipeline`

[2023.07.12 - v2.4.3 - Chunel]
* Optimize `CStatus` and add exception location information

[2023.09.05 - v2.5.0 - Chunel]
* Provide the perf feature for `pipeline` performance analysis
* Provide the timeout mechanism for `element`
* Provide the `some` feature to optimize asynchronous execution of `pipeline`

[2023.09.15 - v2.5.1 - Chunel]
* Provide the `fence` feature
* Provide the `coordinator` feature

[2023.11.06 - v2.5.2 - Chunel]
* Optimize the `message` feature, allow configuring how to handle blocked writes, and reduce memory copy count
* Add `example` content with simple implementations for different industries
* Optimize scheduling performance

[2023.11.15 - v2.5.3 - Chunel]
* Provide `proto` definition files
* Add the `mutable` feature and provide dependency registration syntax sugar

[2024.01.05 - v2.5.4 - Chunel]
* Provide `test` content, including performance and functional test cases
* Optimize the `event` mechanism and support asynchronous waiting

[2024.07.18 - v2.6.0 - [PaPaPig-Melody](https://github.com/PaPaPig-Melody), Chunel]
* Provide topological execution for `pipeline`
* Provide a method to determine whether dependencies exist between `element`s
* Provide Bazel build support
* Optimize the perf feature

[2024.09.17 - v2.6.1 - Chunel]
* Provide static execution for `pipeline` and a micro-task mechanism based on static execution
* Provide `pipeline` pruning to remove duplicate dependencies between `element`s
* Provide a method for `element` to remove dependencies
* Optimize the `event` mechanism so asynchronous events can wait for completion
* Release the [CGraph-lite](https://github.com/ChunelFeng/CGraph-lite) project, providing simple DAG construction and parameter passing. Its APIs are fully compatible and can be seamlessly switched to this project

[2024.11.16 - v2.6.2 - Chunel]
* Optimize parameter mutex mechanism and retrieval performance
* Fix abnormal waiting in auxiliary threads
* Update `tutorial` content

[2025.03.16 - v3.0.0 - Chunel]
* Provide the Python version
* Provide the `stage` feature for synchronized running between `element`s
* Update `tutorial` content

[2025.05.04 - v3.1.0 - Chunel]
* Provide the full-featured Python version
* Provide Python and C++ hybrid programming
* Provide Python packaging and support installation with `pip3 install PyCGraph`

[2025.06.15 - v3.1.1 - Chunel]
* Provide C# and Java versions
* Provide the `CODE_OF_CONDUCT.md` document

[2025.09.06 - v3.1.2 - Chunel]
* Provide the Go version
* Optimize the `message` feature
* Optimize the `aspect` feature

[2025.11.08 - v3.2.2 - Chunel]
* Provide local save and load features
* Rename `PyCGraph` to `pycgraph` and support installation with `pip3 install pycgraph`

[2026.03.24 - v3.2.3 - Chunel]
* Optimize the `message` feature
* Optimize stability

[2026.05.10 - v3.2.4 - Chunel]
* Optimize the `pycgraph` feature

</details>

------------
<details>
<summary><b>Appendix 2. Thanks</b></summary>

* Thanks to the [Doocs WeChat official account](https://mp.weixin.qq.com/mp/appmsgalbum?__biz=MzIxNjA5ODQ0OQ==&action=getalbum&album_id=1989460124624551937&scene=173&from_msgid=2654703194&from_itemidx=1&count=3&nolastread=1#wechat_redirect) for publishing the related introduction document. Welcome to join the [Doocs open-source community](https://github.com/doocs)
* Thanks to the introduction and recommendation from HelloGithub magazine: [HelloGithub Issue 70](https://github.com/521xueweihan/HelloGitHub/blob/master/content/HelloGitHub70.md)

<p align="center"><img src="https://github.com/ChunelFeng/CGraph/blob/main/doc/image/HelloGithub%20Logo.gif"/></p>

* Thanks to the recommendation from [awesome-cpp](https://github.com/fffaraz/awesome-cpp), we all know, it is the most authoritative recommendation list for cpp project in the world
* Thanks to the recommendation from `Taskflow Group`: [awesome-parallel-computing](https://github.com/taskflow/awesome-parallel-computing), and we always treat [taskflow](https://github.com/taskflow/taskflow) as a role model
* Thanks to the recommendation from [awesome-workflow-engines](https://github.com/meirwah/awesome-workflow-engines)
* Thanks to all developers in [CONTRIBUTORS](https://github.com/ChunelFeng/CGraph/blob/main/CONTRIBUTORS.md) for their contributions to the project
* Thanks to all friends who have provided suggestions and advice for the `CGraph` project. They are not listed one by one here. Everyone is always welcome to join and build together

</details>

------------
<details>
<summary><b>Appendix 3. Contact</b></summary>

* WeChat: ChunelFeng (You are welcome to scan the QR code above and add the author as a friend. Please briefly note your personal information ^_^)
* Email: chunel@foxmail.com
* Source Code: https://github.com/ChunelFeng/CGraph
* Forum: www.chunel.cn

</details>
