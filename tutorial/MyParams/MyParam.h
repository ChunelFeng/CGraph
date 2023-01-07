/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyParam.h
@Time: 2021/6/13 3:20 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYPARAM_H
#define CGRAPH_MYPARAM_H

#include "../../src/CGraph.h"

/**
 * MyParam 是GParam的具体实现类
 * 本例中使用 struct 类型，是为了使得 参数权限 默认为是public类型
 * 如果使用 class 类型，请注意 参数权限 默认是private类型信息
 */
struct MyParam : public CGraph::GParam {
    /**
     * reset方法，在pipeline执行一次结束的时候被调用。
     * 如果是pipeline多次执行，并且依赖之前pipeline运行的结果（如，通过iCount值，记录pipeline执行了多少次）
     * reset中，不要实现重置参数逻辑即可
     */
    CVoid reset() override {
        iValue = 0;    // pipeline 执行一次结束后，iValue值清0；iCount值保留，可带入下一次pipeline执行
    }

    int iValue { 0 };
    int iCount { 0 };
};

#endif // CGRAPH_MYPARAM_H
