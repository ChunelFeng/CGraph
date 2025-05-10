<p align="left">
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/langs/C++,Python/cyan?list=1" alt="languages"></a>
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/os/MacOS,Linux,Windows/cyan?list=1" alt="os"></a>
  <a href="https://github.com/ChunelFeng/CGraph/stargazers"><img src="https://badgen.net/github/stars/ChunelFeng/CGraph?color=cyan" alt="stars"></a>
  <a href="https://github.com/ChunelFeng/CGraph/network/members"><img src="https://badgen.net/github/forks/ChunelFeng/CGraph?color=cyan" alt="forks"></a>
  <a href="https://badge.fury.io/py/PyCGraph"><img src="https://badge.fury.io/py/PyCGraph.svg" alt="pypi"></a>
  <a href="https://www.codefactor.io/repository/github/chunelfeng/cgraph/overview/main"><img src="https://www.codefactor.io/repository/github/chunelfeng/cgraph/badge/main" alt="CodeFactor" /></a>
</p>

[![awesome-cpp](https://badgen.net/badge/icon/awesome-cpp/purple?icon=awesome&label&color)](https://github.com/fffaraz/awesome-cpp)
[![HelloGithub](https://badgen.net/badge/icon/HelloGithub/purple?icon=awesome&label&color)](https://github.com/521xueweihan/HelloGitHub/blob/master/content/HelloGitHub70.md)

[中文](README.md) | English Readme | [deepwiki](https://deepwiki.com/ChunelFeng/CGraph)

<h1 align="center">
  CGraph Readme
</h1>

## 1. Introduction

CGraph, short for <b>C</b>olor <b>Graph</b>, is a cross-platform DAG computing framework. It is written by C++11 without any third-party dependencies, Python APIs are also supported.

With the scheduling via `GPipeline`, the purpose of sequential and concurrent executing elements is realized. You only need to inherit `GNode` class, implement the `run()` method in the subclass, and set the dependencies as needed to achieve the graphical execution of tasks.

At the same time, you can also control the graph conditional judgment, loop or concurrent execution logic by setting various `GGroup`s, which containing multi-node information by themselves.

You can transfer your params in many scenes. It is also possible to extend the functions of the above elements horizontally by adding `GAspect`, to enhance the functions of individual nodes by introducing various `GAdapter`, or to enrich pipeline schedule by `GEvent`.

![CGraph Skeleton](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Skeleton.jpg)
<br>

## 2. Demo

> C++ version

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
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");
    pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");

    pipeline->process();

    GPipelineFactory::remove(pipeline);

    return 0;
}
```

![CGraph Demo](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Demo.jpg)
<br>
As is shown on the picture, run `a` firstly. Then, run `b` and `c` parallelized. Run `d` at last after `b` and `c` finished.

> Python version
```python
import time
from datetime import datetime

from PyCGraph import GNode, GPipeline, CStatus


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
