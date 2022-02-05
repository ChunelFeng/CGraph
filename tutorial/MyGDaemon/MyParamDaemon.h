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
#include "../MyGParam/MyParam.h"

class MyParamDaemon : public CGraph::GDaemon {

public:
    CVoid daemonTask() override {
        auto param = CGRAPH_GET_GPARAM(MyParam, "param1")    // 从注册的pipeline中获取参数信息
        if (nullptr == param) {
            CGraph::CGRAPH_ECHO("----> [MyParamDaemon] error, enter no find param path ...");
            return;
        }

        // 根据实际情况，选择不上锁，或者上读/写锁
        CGraph::CGRAPH_ECHO("----> [MyParamDaemon] iCount is [%d], just suppose this value is show on dashboard every [%u] ms",
                            param->iCount, this->getInterval());
    }
};


#endif //CGRAPH_MYPARAMDAEMON_H
