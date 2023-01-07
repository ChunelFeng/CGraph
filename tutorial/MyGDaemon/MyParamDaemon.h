/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyParamDaemon.h
@Time: 2022/2/3 5:13 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYPARAMDAEMON_H
#define CGRAPH_MYPARAMDAEMON_H

#include "../../src/CGraph.h"
#include "../MyParams/MyParam.h"
#include "../MyParams/MyConnParam.h"

class MyParamDaemon : public CGraph::GDaemon {
public:
    CVoid daemonTask(CGraph::GDaemonParamPtr param) override {
        /**
         * 从注册的pipeline中获取参数信息
         * 注意，这里是GParam类型
         */
        auto* myParam = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1")
        // 根据实际情况，选择不上锁，或者上读/写锁
        CGraph::CGRAPH_ECHO("----> [MyParamDaemon] iCount is [%d], just suppose this value is show on dashboard every [%u] ms",
                            myParam->iCount, this->getInterval());

        /**
         * 获取任务外部传入的参数信息
         * 需要强转成外部传入的类型，方法如下：
         */
        auto connParam = dynamic_cast<MyConnParam *>(param);
        if (connParam) {
            // 对应有参数传入的分支
            CGraph::CGRAPH_ECHO("----> [MyParamDaemon] input param, address is [%s : %d]",
                                connParam->ip_.c_str(), connParam->port_);
        } else {
            // 对应无参数传入的分支
            CGraph::CGRAPH_ECHO("----> [MyParamDaemon] no input param info.");
        }
    }
};


#endif //CGRAPH_MYPARAMDAEMON_H
