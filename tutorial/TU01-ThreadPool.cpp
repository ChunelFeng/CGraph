/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: TU01-ThreadPool.cpp
@Time: 2021/9/2 11:12 下午
@Desc: 本例主要演示，threadpool工具的使用方法
***************************/

#include "../src/CGraph.h"
#include "MyUFunction/MyFunction.h"


void tutorial_threadpool_1() {
    /**
     * 依次向线程池中传入：
     * 1、普通函数
     * 2、静态函数
     * 3、类成员函数
     * 4、类成员静态函数
     */
    UThreadPoolPtr tp = UThreadPoolSingleton::get();
    int i = 6, j = 3;
    std::string str = "5";
    MyFunction mf;

    auto r1 = tp->commit([i, j] { return add(i, j); });    // 可以通过lambda表达式传递函数
    std::future<float> r2 = tp->commit(std::bind(minusBy5, 8.5f));    // 可以传入任意个数的入参
    auto r3 = tp->commit(std::bind(&MyFunction::pow2, mf, str));    // 返回值可以是任意类型
    std::future<int> r4 = tp->commit(std::bind(&MyFunction::divide, i, j));    // 返回值实际上是std::future<T>类型

    std::cout << r1.get() << std::endl;    // 返回值可以是int类型
    std::cout << r2.get() << std::endl;    // 等待r2对应函数执行完毕后，再继续执行。不调用get()为不等待
    std::cout << r3.get() << std::endl;    // 返回值也可是string或其他任意类型
    std::cout << r4.get() << std::endl;
}


void tutorial_threadpool_2() {
    UThreadPoolPtr tp = UThreadPoolSingleton::get();
    // 并发打印0~100之间的数字
    for (int i = 0; i < 100; i++) {
        tp->commit([i] {
            std::cout << i << std::endl;
        });
    }
}


int main() {
    CGRAPH_ECHO("tutorial_threadpool_1 begin.");
    tutorial_threadpool_1();

    CGRAPH_ECHO("tutorial_threadpool_2 begin.");
    tutorial_threadpool_2();
    return 0;
}
