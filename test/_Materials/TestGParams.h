/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: TestGParams.h
@Time: 2024/1/1 16:58
@Desc: 
***************************/

#ifndef CGRAPH_TESTGPARAMS_H
#define CGRAPH_TESTGPARAMS_H

#include "TestCommonDefine.h"

class TestGMessageParam : public CGraph::GMessageParam {
public:
    unsigned int num_ {0};
};


class TestAdd1GParam : public CGraph::GParam {
public:
    int cnt_ = 0;
};

#endif //CGRAPH_TESTGPARAMS_H
