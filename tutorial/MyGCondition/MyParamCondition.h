/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyParamCondition.h
@Time: 2021/6/19 11:58 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYPARAMCONDITION_H
#define CGRAPH_MYPARAMCONDITION_H

#include "CGraph.h"
#include "../MyParams/MyParam.h"

class MyParamCondition : public CGraph::GCondition {
public:
    /**
     * 在这里主要演示condition中可以通过获取上方参数的形式，
     * 来决定执行执行当前的第几个逻辑
     * @return
     */
    CIndex choose () override {
        auto* myParam = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1")
        int cnt = 0;
        {
            CGRAPH_PARAM_READ_CODE_BLOCK(myParam)    // 如果当前算子，跟其他相关依赖算子不存在并行关系，则参数可以直接使用，不需要加锁
            cnt = myParam->iCount;
        }

        auto relation = getRelation();
        int range = (int)relation.children_.size();
        CGRAPH_THROW_EXCEPTION_BY_CONDITION(range == 0, getName() + " has 0 element.");
        return (cnt % range);
    }
};

#endif //CGRAPH_MYPARAMCONDITION_H
