/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyCondition.h
@Time: 2021/6/19 10:17 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYCONDITION_H
#define CGRAPH_MYCONDITION_H

#include "../../src/GraphCtrl/GraphInclude.h"

class MyCondition : public GCondition {

public:
    int choose () override {
        return 1;
    }
};

#endif // CGRAPH_MYCONDITION_H
