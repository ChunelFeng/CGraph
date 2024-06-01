<h1 align="center">
  CGraph 编译说明
</h1>

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

* Bazel编译方式(Linux/MacOS/Windows)
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ bazel build //tutorial:T01-Simple -c dbg && bazel run //tutorial:T01-Simple  # 编译并运行tutorial路径下的T01-Simple（debug版本）
  $ bazel build //tutorial/... -c opt      # 编译tutorial路径下的所有targets（release版本）
  ```