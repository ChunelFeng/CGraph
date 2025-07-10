<h1 align="center">
  CGraph 编译说明
</h1>

### C++ 版本

* 本工程支持`MacOS`、`Linux`、`Windows`和`Android`系统，无任何第三方依赖。默认使用C++11版本，推荐使用C++17版本，暂不支持C++11以下的版本

* 使用`CLion`(推荐)作为IDE的开发者，打开`CMakeLists.txt`文件作为工程，即可编译通过。本项目已经获得 [Jetbrains 开源开发许可证](https://www.jetbrains.com/zh-cn/community/opensource/#support )，感谢 Jetbrains 提供优秀的IDE，和对项目的认可

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

* Cmake编译方式(Linux/MacOS/Windows)
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ cmake . -Bbuild
  $ cd build && make -j8
  $ ./tutorial/T00-HelloCGraph              # 运行 T00-HelloCGraph
  ```

* Bazel编译方式(Linux/MacOS/Windows)
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ bazel build //tutorial/...              # 编译 tutorial路径下的所有targets
  $ bazel run //tutorial:T00-HelloCGraph    # 运行 T00-HelloCGraph
  ```

* Xmake编译方式(Linux/MacOS/Windows)
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ xmake build                             # 编译 tutorial 中的所有项目
  $ xmake run T00-HelloCGraph               # 运行 T00-HelloCGraph
  ```

* 提供online版本的编译调试环境，点击进入页面：[CGraph env online](https://gitpod.io/#/github.com/ChunelFeng/CGraph) ，通过github账号登录。进入后，输入以下指令，即可编译通过，并查看执行结果
  ```shell
  $ sudo apt-get install cmake -y           # 安装cmake
  $ ./CGraph-build.sh                       # 编译CGraph工程，生成的内容在同级/build/文件夹中
  $ ./build/tutorial/T00-HelloCGraph        # 运行T00-HelloCGraph，并且在终端输出 Hello, CGraph.
  ```

---

* 在其他使用 CMakeLists.txt 构建的项目中，通过 FetchContent 作为三方库引入
  ```cmake
  set(CGRAPH_BUILD_TUTORIALS OFF CACHE BOOL "" FORCE)
  set(CGRAPH_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(CGRAPH_BUILD_FUNCTIONAL_TESTS OFF CACHE BOOL "" FORCE)
  set(CGRAPH_BUILD_PERFORMANCE_TESTS OFF CACHE BOOL "" FORCE)

  Include(FetchContent)
  FetchContent_Declare(
    CGraph
    GIT_REPOSITORY https://github.com/ChunelFeng/CGraph.git
    GIT_TAG main
    GIT_SHALLOW true
  )

  FetchContent_MakeAvailable(CGraph)
  target_include_directories(${PROJECT_NAME} PRIVATE ${CGraph_SOURCE_DIR}/src)
  target_link_libraries(${PROJECT_NAME} PRIVATE CGraph)
  ```

---

### Python 版本
* MacOS 和 Windows 用户可以通过命令安装（推荐）
  ```shell
  $ pip3 install PyCGraph                  # 直接通过 pip3 安装
  $ python3 -c "import PyCGraph"           # 验证 PyCGraph 成功安装
  ```

* 通过源码安装(Linux/MacOS/Windows)，使用前需要提前安装`python3` `pybind11`和`setuptools`
  * 通过生成`.whl`文件安装（推荐）
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph/python                       # 进入对应文件夹
  $ python3 setup.py bdist_wheel           # 生成 *.whl 安装包
  $ pip3 install dist/PyCGraph-xxx.whl     # 安装 *.whl 安装包，xxx 信息与版本和系统有关
  $ python3 tutorial/T00-HelloCGraph.py    # 运行T00-HelloCGraph.py，并且在终端输出 Hello, PyCGraph.
  ```
  * 本地直接安装
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph/python                       # 进入对应文件夹
  $ python3 setup.py install               # 安装 PyCGraph
  $ python3 tutorial/T00-HelloCGraph.py    # 运行T00-HelloCGraph.py，并且在终端输出 Hello, PyCGraph.
  ```
