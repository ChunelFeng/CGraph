/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyWriteParamNode.h
@Time: 2021/6/13 3:22 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYWRITEPARAMNODE_H
#define CGRAPH_MYWRITEPARAMNODE_H

#include "../../src/CGraph.h"
#include "../MyParams/MyParam.h"

class MyWriteParamNode : public CGraph::GNode {
public:
    CStatus init () override {
        CStatus status;
        /**
         * 推荐在init()中，将可能用到的参数创建好。也支持在run的时候创建
         * 支持在任意节点创建，任意节点读取同类型（MyParam）同名（"param1"）参数
         */
        status = CGRAPH_CREATE_GPARAM(MyParam, "param1")
        return status;
    }

    CStatus run () override {
        auto* myParam = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1")
        int val = 0;
        int cnt = 0;
        {
            CGRAPH_PARAM_WRITE_CODE_BLOCK(myParam)
            /**
             * 建议将需要的内容获取出来，然后在 CODE_BLOCK 外进行与param无关的参数加工（本例中为：打印信息）。
             * 这样做的好处，是可以尽可能减少 CODE_BLOCK 的范围
             */
            val = ++myParam->iValue;    // 对param中的信息，进行写入
            cnt = ++myParam->iCount;
        }

        /* 执行时，仅依赖val值，跟当前param1->iValue值无关，可以放到 CODE_BLOCK 外部 */
        CGraph::CGRAPH_ECHO("[%s], iValue value is : [%d], iCount value is [%d] ... ",
                            this->getName().c_str(), val, cnt);

        return CStatus();
    }
};

#endif //CGRAPH_MYWRITEPARAMNODE_H
