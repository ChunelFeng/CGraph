/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyWriteParamNode.h
@Time: 2021/6/13 3:22 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYWRITEPARAMNODE_H
#define CGRAPH_MYWRITEPARAMNODE_H

#include "../../src/GraphCtrl/GraphInclude.h"
#include "../MyGParam/MyParam1.h"

class MyWriteParamNode : public GNode {
public:
    CSTATUS init () {
        CSTATUS status = STATUS_OK;
        /**
         * 推荐在init()中，将可能用到的参数创建好。也支持在run的时候创建
         * 支持在任意节点创建，任意节点读取同类型（MyParam1）同名（"myParam1"）参数
         */
        status = this->createGParam<MyParam1>("param1");
        return status;
    }

    CSTATUS run () {
        MyParam1* param1 = this->getGParam<MyParam1>("param1");
        if (nullptr == param1) {
            return STATUS_ERR;    // 如果获取myParam1失败，则返回错误码信息
        }

        int val = 0;
        {
            CGRAPH_PARAM_WRITE_CODE_BLOCK(param1)
            param1->iValue++;    // 对param1中的信息，进行写入
            /**
             * 建议将需要的内容获取出来，然后在 CODE_BLOCK 外进行与param无关的参数加工（本例中为：打印信息）。
             * 这样做的好处，是可以尽可能减少 CODE_BLOCK 的范围
             */
            val = param1->iValue;
        }

        /* 执行时，仅依赖val值，跟当前param1->iValue值无关，可以放到 CODE_BLOCK 外部 */
        CGRAPH_ECHO("[%s], iValue value is : [%d] ... ", this->getName().c_str(), val);

        return STATUS_OK;
    }
};

#endif //CGRAPH_MYWRITEPARAMNODE_H
