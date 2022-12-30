/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: TU01-ThreadPool.cpp
@Time: 2021/9/2 11:12 下午
@Desc: 本例主要演示，threadpool工具的使用方法
***************************/

#include "../src/CGraph.h"
#include "MyUtils/MyFunction.h"

using namespace CGraph;

void tutorial_threadpool_1(UThreadPoolPtr tp) {
    /**
     * 依次向线程池中传入：
     * 1、普通函数
     * 2、静态函数
     * 3、类成员函数
     * 4、类成员静态函数
     */
    int i = 6, j = 3;
    std::string str = "5";
    MyFunction mf;

    auto r1 = tp->commit([i, j] { return add(i, j); });    // 可以通过lambda表达式传递函数
    std::future<float> r2 = tp->commit(std::bind(minusBy5, 8.5f));    // 可以传入任意个数的入参
    auto r3 = tp->commit(std::bind(&MyFunction::pow2, mf, str));    // 返回值可以是任意类型
    std::future<int> r4 = tp->commit([i, j] { return MyFunction::divide(i, j); });    // 返回值实际上是std::future<T>类型

    std::cout << r1.get() << std::endl;    // 返回值可以是int类型
    std::cout << r2.get() << std::endl;    // 等待r2对应函数执行完毕后，再继续执行。不调用get()为不等待
    std::cout << r3.get() << std::endl;    // 返回值也可是string或其他任意类型
    std::cout << r4.get() << std::endl;
}


void tutorial_threadpool_2(UThreadPoolPtr tp) {
    /**
     * 通过添加工作组(taskGroup)来执行任务
     */
    UTaskGroup taskGroup;

    /** 添加一个不耗时的任务 */
    int i = 1, j = 2, k = 3;
    auto hcg = [] { CGRAPH_ECHO("Hello, CGraph."); };
    taskGroup.addTask(hcg);

    /** 添加一个耗时为1000ms的任务 */
    taskGroup.addTask([i, j] {
        int result = i + j;
        CGRAPH_SLEEP_MILLISECOND(1000)
        CGRAPH_ECHO("sleep for 1 second, [%d] + [%d] = [%d], run success.", i, j, result);
    });

    taskGroup.addTask([i, j, k] {
        int result = i - j + k;
        CGRAPH_SLEEP_MILLISECOND(2000)
        CGRAPH_ECHO("sleep for 2 second, [%d] - [%d] + [%d] = [%d], run success.", i, j, k, result);
        return result;    // submit接口，不会对线程函数返回值进行判断。如果需要判断，考虑commit方式
    });

    /** 如果添加耗时3000ms的任务，则整体执行失败 */
    /* taskGroup.addTask([] {
          CGRAPH_SLEEP_MILLISECOND(3000)
     }); */

    /**
     * 可以添加执行task group结束后的回调信息
     * 其中sts是task group整体执行结果的返回值信息
     * */
    /* taskGroup.setOnFinished([] (const CStatus& sts) {
        if(sts.isOK()) {
            CGRAPH_ECHO("task group run success.");
        } else {
            CGRAPH_ECHO("task group run failed, error info is [%s].", sts.getInfo().c_str());
        }
    }); */

    /**
     * 设定超时时间=2500ms，确保以上任务能顺利执行完成
     * 如果加入sleep(3000)的任务，则也会在2500ms的时候退出
     * 并且在status中提示超时信息
     * */
    CStatus status = tp->submit(taskGroup, 2500);
    CGRAPH_ECHO("task group run status is [%d].", status.getCode());
}


void tutorial_threadpool_3(UThreadPoolPtr tp) {
    /**
     * 并发打印0~100之间的数字
     * 使用commit和submit函数的区别，主要在于：
     * 1，commit()属于非阻塞执行，是将线程函数执行的结果以future的类型返回，交由上层处理
     * 2，submit()属于阻塞顺序执行，是在内部处理好超时等信息并作为结果返回，抛弃线程函数自身返回值
     * 3，不需要线程函数返回值，并且不需要判断超时信息的场景，两者无区别（如下例）
     */
    const int size = 100;
    CGRAPH_ECHO("thread pool task submit version : ");
    for (int i = 0; i < size; i++) {
        tp->submit([i] { std::cout << i << " "; });    // 可以看到，submit版本是有序执行的。如果需要想要无序执行，可以通过创建taskGroup的方式进行，或者使用commit方法
    }
    CGRAPH_SLEEP_SECOND(1)    // 等待上面函数执行完毕，以便于观察结果。无实际意义
    std::cout << "\r\n";

    CGRAPH_ECHO("thread pool task group submit version : ");
    UTaskGroup taskGroup;
    for (int i = 0; i < size; i++) {
        taskGroup.addTask([i] { std::cout << i << " "; });    // 将任务放到一个taskGroup中，并发执行。执行的结果是无序的
    }
    tp->submit(taskGroup);
    CGRAPH_SLEEP_SECOND(1)
    std::cout << "\r\n";

    CGRAPH_ECHO("thread pool task commit version : ");
    for (int i = 0; i < size; i++) {
        tp->commit([i] { std::cout << i << " "; });    // commit版本，是无序执行的
    }
    CGRAPH_SLEEP_SECOND(1)
    std::cout << "\r\n";
}


int main() {
    auto pool = std::make_unique<UThreadPool>();    // 构造一个线程池类的智能指针
    CGRAPH_ECHO("======== tutorial_threadpool_1 begin. ========");
    tutorial_threadpool_1(pool.get());

    CGRAPH_ECHO("======== tutorial_threadpool_2 begin. ========");
    tutorial_threadpool_2(pool.get());

    CGRAPH_ECHO("======== tutorial_threadpool_3 begin. ========");
    tutorial_threadpool_3(pool.get());
    return 0;
}
