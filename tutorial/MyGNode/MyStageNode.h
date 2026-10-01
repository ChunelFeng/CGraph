/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyStageNode.h
@Time: 2024/12/15 18:46
@Desc: 
***************************/

#ifndef CGRAPH_MYSTAGENODE_H
#define CGRAPH_MYSTAGENODE_H

#include "CGraph.h"

static auto kStageKey = "stage";

template<int before, int after>
class MyStageNode : public CGraph::GNode {
public:
    CStatus run() override {
        CGRAPH_SLEEP_SECOND(before);
        CGraph::CGRAPH_ECHO("[%s] wait for stage", getName().c_str());

        // 等到所有的流程，都完成等待之后，会统一往下执行
        waitGStage(kStageKey);
        CGraph::CGRAPH_ECHO("[%s] finish stage, before [%d]s, after [%d]s", getName().c_str(), before, after);
        CGRAPH_SLEEP_SECOND(after);
        return CStatus();
    }
};

#endif //CGRAPH_MYSTAGENODE_H
