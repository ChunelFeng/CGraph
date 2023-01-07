/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T13-Daemon.cpp
@Time: 2022/2/3 11:31 上午
@Desc: 本例子主要展示，添加Daemon节点完成对pipeline的实时信息获取
***************************/

#include "MyGDaemon/MyMonitorDaemon.h"
#include "MyGDaemon/MyParamDaemon.h"
#include "MyGDaemon/MyTemplateDaemon.h"
#include "MyGNode/MyNode1.h"
#include "MyGNode/MyWriteParamNode.h"
#include "MyParams/MyConnParam.h"

using namespace CGraph;

void tutorial_daemon() {
    GPipelinePtr pipeline = GPipelineFactory::create();    // 创建一个pipeline，用于执行图框架信息
    GElementPtr a, b = nullptr;

    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    pipeline->registerGElement<MyWriteParamNode>(&b, {a}, "writeParamNodeB");    // writeParamNodeB中的值，每隔1s改变一次

    MyConnParam connParam;
    connParam.ip_ = "127.0.0.1";
    connParam.port_ = 6666;

    /** 添加两个daemon信息，随pipeline执行 */
    pipeline->addGDaemon<MyMonitorDaemon>(4000)   // 间隔4s执行
            ->addGDaemon<MyParamDaemon, MyConnParam>(3500, &connParam)    // 间隔3500ms执行，并且传入参数
            ->addGDaemon<MyTemplateDaemon<int>>(2750, 300);    // 添加模板daemon信息

    /** 长时间执行，观察守护器执行状态 */
    pipeline->process(20);
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_daemon();
    return 0;
}