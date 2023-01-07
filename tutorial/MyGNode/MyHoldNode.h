/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyHoldNode.h
@Time: 2022/4/6 21:19
@Desc: 
***************************/

#ifndef CGRAPH_MYHOLDNODE_H
#define CGRAPH_MYHOLDNODE_H

#include "../../src/CGraph.h"
#include "../MyParams/MyParam.h"

static const char* HOLD_PARAM_NAME = "hold-param";

class MyHoldNode : public CGraph::GNode {
public:
    CStatus init() override {
        CStatus status = CGRAPH_CREATE_GPARAM(MyParam, HOLD_PARAM_NAME)
        return status;
    }

    CStatus run() override {
        auto param = CGRAPH_GET_GPARAM(MyParam, HOLD_PARAM_NAME)
        CGRAPH_ASSERT_NOT_NULL(param)

        param->iValue++;
        CGraph::CGRAPH_ECHO("iCount value is [%d]", param->iValue);
        return CStatus();
    }

    CBool isHold() override {
        /**
         * 针对当前场景，添加 hold逻辑
         * 当 hold逻辑被满足的时候，会重复执行当前节点，直到返回false为止
         */
        auto param = CGRAPH_GET_GPARAM(MyParam, HOLD_PARAM_NAME)
        if (nullptr == param) {
            return false;    // 如果未读取到参数，则直接结束，不继续执行了
        }

        CGraph::CGRAPH_ECHO("enter hold path, iValue = [%d]", param->iValue);
        return param->iValue < 5;    // 当iCount值小于3的时候，此节点会持续执行
    }
};

#endif //CGRAPH_MYHOLDNODE_H
