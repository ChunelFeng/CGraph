/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyPipelineParamAspect.h
@Time: 2021/10/5 11:48 上午
@Desc: 
***************************/

#ifndef CGRAPH_MYPIPELINEPARAMASPECT_H
#define CGRAPH_MYPIPELINEPARAMASPECT_H

#include "../../src/CGraph.h"
#include "../MyParams/MyParam.h"

class MyPipelineParamAspect : public CGraph::GAspect {
public:
    CStatus beginRun() override {
        CStatus status;
        auto* pipelineParam = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1")
        /** 遇到并发情况，请考虑加锁保护（参考 T05-Param 中逻辑） */
        int cnt = pipelineParam->iCount;
        CGraph::CGRAPH_ECHO("----> [MyPipelineParamAspect] pipeline param iCount is [%d] before run.", cnt);

        if (cnt < 0) {
            /** 模拟：在切面中，对pipeline中的参数，做一些异常值的处理逻辑
             * 在 beginRun() 切面中 返回非STATUS_OK值，则pipeline停止执行
             * */
            return CStatus("aspect demo error");
        }

        return status;
    }
};

#endif //CGRAPH_MYPIPELINEPARAMASPECT_H
