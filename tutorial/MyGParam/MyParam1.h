/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyParam1.h
@Time: 2021/6/13 3:20 下午
@Desc: 
***************************/


#ifndef CGRAPH_MYPARAM1_H
#define CGRAPH_MYPARAM1_H

#include "../../src/GraphCtrl/GraphInclude.h"

struct MyParam1 : public GParam {
    void reset() override {
        iValue = 0;
        fValue = 0.0f;
    }

    int iValue {0};
    float fValue {0.0f};
};

#endif //CGRAPH_MYPARAM1_H
