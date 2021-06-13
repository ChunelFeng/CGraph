/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyParamNode1.h
@Time: 2021/6/13 3:22 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYPARAMNODE1_H
#define CGRAPH_MYPARAMNODE1_H

#include "../../src/GraphCtrl/GraphInclude.h"
#include "../MyGParam/MyParam1.h"

class MyParamNode1 : public GNode {
public:
    CSTATUS init () {
        CSTATUS status = STATUS_OK;
        status = this->createGParam<MyParam1>("myParam1");    // 推荐在init的时候，将可能用到的参数创建好
        return status;
    }

    CSTATUS run () {
        CSTATUS status = STATUS_OK;

        MyParam1* param1 = this->getGParam<MyParam1>("myParam1");
        {
            CGRAPH_PARAM_WRITE_REGION(param1)
            std::cout << "--> write : " << ++param1->iValue << std::endl;
        }

        return status;
    }
};

#endif //CGRAPH_MYPARAMNODE1_H
