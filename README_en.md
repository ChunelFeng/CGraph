<p align="left">
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/langs/C++/cyan?list=1" alt="languages"></a>
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/os/MacOS,Linux,Windows/cyan?list=1" alt="os"></a>
  <a href="https://github.com/ChunelFeng/CGraph/stargazers"><img src="https://badgen.net/github/stars/ChunelFeng/CGraph?color=cyan" alt="stars"></a>
  <a href="https://github.com/ChunelFeng/CGraph/network/members"><img src="https://badgen.net/github/forks/ChunelFeng/CGraph?color=cyan" alt="forks"></a>
</p>

[![awesome-cpp](https://badgen.net/badge/icon/awesome-cpp/purple?icon=awesome&label&color)](https://github.com/fffaraz/awesome-cpp)
[![HelloGithub](https://badgen.net/badge/icon/HelloGithub/purple?icon=awesome&label&color)](https://github.com/521xueweihan/HelloGitHub/blob/master/content/HelloGitHub70.md)

[中文](README.md) | English Readme

<h1 align="center">
  CGraph Readme
</h1>

## 1. Introduction

CGraph, short for <b>C</b>olor <b>Graph</b>, is a cross-platform DAG computing framework without any third-party dependencies. With the scheduling via `GPipeline`, the purpose of sequential and concurrent executing elements is realized.

You only need to inherit `GNode` class, implement the `run()` method in the subclass, and set the dependencies as needed to achieve the graphical execution of tasks.

At the same time, you can also control the graph conditional judgment, loop or concurrent execution logic by setting various `GGroup`s, which containing multi-node information by themselves.

You can transfer your params in many scenes. It is also possible to extend the functions of the above elements horizontally by adding `GAspect`, to enhance the functions of individual nodes by introducing various `GAdapter`, or to enrich pipeline schedule by `GEvent`.

![CGraph Skeleton](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Skeleton.jpg)
<br>

## 2. Compile
* This project supports MacOS, Linux, and Windows systems without any third-party dependencies. C++11 is default and lowest version, C++17 is recommended.
  
* For developers using `CLion` as IDE within all platform, open the `CMakeLists.txt` file as project to compile.

* Developers on Windows system, using `Visual Studio`(2013 version at least) as IDE, with cmake, enter commands as flowers to build `CGraph.sln` file.
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ cmake . -Bbuild
  ```

* Developers on MacOS system, using `Xcode` as IDE, with cmake, enter commands as flowers to build `CGraph.xcodeproj` file.
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ mkdir build && cd build
  $ cmake .. -G Xcode
  ```

* Developers on Linux system, enter commands as flowers to compile.
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ cmake . -Bbuild
  $ cd build
  $ make -j8
  ```

* Compile online, enter [CGraph env online](https://gitpod.io/#/github.com/ChunelFeng/CGraph), log in with your Github id, enter commands as flowers to compile and run your first tutorial.
  ```shell
  $ sudo apt-get install cmake -y
  $ ./CGraph-build.sh
  $ ./build/tutorial/T00-HelloCGraph
  ```

## 3. Demo

#### MyNode.h
```cpp
#include "CGraph.h"

class MyNode1 : public CGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        printf("[%s], Sleep for 1 second ... \n", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(1)
        return status;
    }
};


class MyNode2 : public CGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        printf("[%s], Sleep for 1 second ... \n", this->getName().c_str());
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
    /* build a pipeline */
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    /* register node with dependency info */
    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");    // register nodeA with no dependency
    status += pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");    // b depends a
    status += pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    status += pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");    // d depends b and c
    if (!status.isOK()) {
        return;
    }

    /* run dag pipeline */
    status = pipeline->process();
    GPipelineFactory::remove(pipeline);

    return 0;
}
```

![CGraph Demo](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Demo.jpg)
<br>
As is shown on the picture, run `a` firstly. Then, run `b` and `c` parallelized. Run `d` at last after `b` and `c` finished.





