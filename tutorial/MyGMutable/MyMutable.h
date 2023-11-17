/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyMutable.h
@Time: 2023/11/15 21:22
@Desc: 
***************************/

#ifndef CGRAPH_MYMUTABLE_H
#define CGRAPH_MYMUTABLE_H

#include "CGraph.h"
#include "../MyParams/MyParam.h"

class MyMutable : public CGraph::GMutable {
public:
    CStatus reshape(CGraph::GElementPtrArr& elements) override {
        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1")
        int count = param->iCount % 4;
        if (0 == count) {
            CGraph::CGRAPH_ECHO("---- run as a->[b,c]");
            (*elements[0])-->elements[1] & elements[2];
        } else if (1 == count) {
            CGraph::CGRAPH_ECHO("---- run as c(*3)->b->a");
            (*elements[2])--*3>elements[1];    // (*elements[2]) 循环3次，并且在 elements[1] 之前执行
            (*elements[1])-->elements[0];
        } else if (2 == count) {
            // 只有在这里，显示描述的element，被激活执行。这里， elements[1] 不执行
            CGraph::CGRAPH_ECHO("---- run as a->c, do not run b");
            (*elements[0])-->elements[2];
        } else if (3 == count) {
            CGraph::CGRAPH_ECHO("---- run as [a,b(*2),c]");
            (*elements[0])--;
            (*elements[1])--*2;
            (*elements[2])--;
        } else {
            CGraph::CGRAPH_ECHO("---- run nothing");    // 理论不会进入这种情况
        }

        return CStatus();
    }
};

#endif //CGRAPH_MYMUTABLE_H
