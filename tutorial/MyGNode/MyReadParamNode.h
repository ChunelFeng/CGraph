/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyReadParamNode.h
@Time: 2021/6/13 3:23 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYREADPARAMNODE_H
#define CGRAPH_MYREADPARAMNODE_H

#include "../../src/CGraph.h"
#include "../MyParams/MyParam.h"

class MyReadParamNode : public CGraph::GNode {
public:
    CStatus run() override {
        auto myParam = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1")    // 获取类型为MyParam且名为"param1"的参数
        int val = 0;
        {
            /* 对需要使用（读或写）参数的位置，加括号{}范围限定，以减少互斥等待时间 */
            CGRAPH_PARAM_READ_CODE_BLOCK(myParam)
            val = myParam->iValue;
        }
        CGraph::CGRAPH_ECHO("[%s], iValue is : [%d] ... ", this->getName().c_str(), val);

        return CStatus();
    }
};

#endif //CGRAPH_MYREADPARAMNODE_H
