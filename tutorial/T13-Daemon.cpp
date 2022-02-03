/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T13-Daemon.cpp
@Time: 2022/2/3 11:31 上午
@Desc: 本例子主要展示，添加Daemon节点完成对pipeline的实时
***************************/

#include "MyGDaemon/MyMonitorDaemon.h"
#include "MyGDaemon/MyParamDaemon.h"
#include "MyGNode/MyNode1.h"
#include "MyGNode/MyWriteParamNode.h"

using namespace CGraph;

void tutorial_daemon() {
    GPipelinePtr pipeline = GPipelineFactory::create();    // 创建一个pipeline，用于执行图框架信息
    GElementPtr a, b = nullptr;

    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    pipeline->registerGElement<MyWriteParamNode>(&b, {}, "writeParamB");

    /** 添加两个daemon信息，随pipeline执行 */
    pipeline->addGDaemon<MyMonitorDaemon>(4)    // 间隔4s执行
            ->addGDaemon<MyParamDaemon>(1, 500);    // 间隔1500ms执行

    /** 长时间执行，观察守护器执行状态 */
    pipeline->process(15);
    GPipelineFactory::remove(pipeline);
}

int main() {
    tutorial_daemon();
    return 0;
}