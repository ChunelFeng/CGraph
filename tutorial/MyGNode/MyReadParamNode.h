/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyReadParamNode.h
@Time: 2021/6/13 3:23 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYREADPARAMNODE_H
#define CGRAPH_MYREADPARAMNODE_H

#include "../../src/GraphCtrl/GraphInclude.h"
#include "../MyGParam/MyParam1.h"

class MyReadParamNode : public GNode {
public:
    CSTATUS run () {
        MyParam1* param1 = this->getGParam<MyParam1>("param1");    // 获取类型为MyParam1且名为"myParam1"的参数
        if (nullptr == param1) {
            return STATUS_ERR;    // 如果没有获取到参数，则返回执行失败
        }

        int val = 0;
        {
            /* 对需要使用（读或写）参数的位置，加括号{}范围限定，以减少互斥等待时间 */
            CGRAPH_PARAM_READ_CODE_BLOCK(param1)
            val = param1->iValue;
        }
        CGRAPH_ECHO("[%s], iValue is : [%d] ... ", this->getName().c_str(), val);

        return STATUS_OK;
    }
};

#endif //CGRAPH_MYREADPARAMNODE_H
