/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyParamNode2.h
@Time: 2021/6/13 3:23 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYPARAMNODE2_H
#define CGRAPH_MYPARAMNODE2_H

#include "../../src/GraphCtrl/GraphInclude.h"

class MyParamNode2 : public GNode {
    CSTATUS run () {
        CSTATUS status = STATUS_OK;
        MyParam1* param = this->getParamManager()->get("myParam");
        param->index += 2;

        return status;
    }
}

#endif //CGRAPH_MYPARAMNODE2_H
