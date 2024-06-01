/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MySendMessageNode.h
@Time: 2022/12/27 00:05
@Desc: 
***************************/

#ifndef CGRAPH_MYSENDMESSAGENODE_H
#define CGRAPH_MYSENDMESSAGENODE_H

#include "CGraph.h"
#include "../MyParams/MyMessageParam.h"

class MySendMessageNode : public CGraph::GNode {
public:
    CStatus run() override {
        /**
         * 可以使用 MyMessageParam mp; 构建值的方式传递
         * 推荐使用 unique_ptr 的方式，进行 send 和 recv
         * 可以减少内存copy次数
         */
        std::unique_ptr<MyMessageParam> mp(new MyMessageParam());

        mp->num = (num_++) * 10;
        mp->info = "this is a test send info, num = " + std::to_string(mp->num);
        /**
         * 在v2.5.1版本，增加了 GMessagePushStrategy 策略，不兼容之前版本
         * 如果需要跟之前逻辑保持一致，直接设定 CGraph::GMessagePushStrategy::WAIT 即可
         * 其他的选项功能，参考 URingBufferPushStrategy(GMessagePushStrategy的别名) 的说明
         */
        CStatus status = CGRAPH_SEND_MPARAM(MyMessageParam, "send-recv", mp, CGraph::GMessagePushStrategy::WAIT)
        return status;
    }

private:
    int num_ = 0;
};


#endif //CGRAPH_MYSENDMESSAGENODE_H
