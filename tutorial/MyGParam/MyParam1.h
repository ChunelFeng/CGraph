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

/**
 * MyParam1是GParam的具体实现类
 * 本例中使用 struct 类型，是为了使得 iValue 默认为是public类型
 * 如果使用 class 类型创建，请注意 iValue 默认是private类型信息
 */
struct MyParam1 : public GParam {
    /**
     * reset方法，在pipeline执行一次结束的时候被调用。
     * 如果是pipeline多次执行，并且依赖之前pipeline运行的结果（如，记录pipeline执行了多少次）
     * reset中，不要实现重置参数逻辑即可
     */
    void reset() override {
        iValue = 0;
    }

    int iValue { 0 };
};

#endif // CGRAPH_MYPARAM1_H
