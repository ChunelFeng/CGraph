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

    CSTATUS init () {
        CSTATUS status = STATUS_OK;
        status = this->getParamManager()->create<MyParam1>("myParam");
        return status;
    }

    CSTATUS run () {
        CSTATUS status = STATUS_OK;
        MyParam1* param = this->getParamManager()->get("myParam");
        param->index += 1;

        return status;
    }
}

#endif //CGRAPH_MYPARAMNODE1_H
