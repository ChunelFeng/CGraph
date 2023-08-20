/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyAsyncNode.h
@Time: 2023/8/20 13:41
@Desc: 
***************************/

#ifndef CGRAPH_MYASYNCNODE_H
#define CGRAPH_MYASYNCNODE_H

#include "CGraph.h"

/**
 * 实现一个异步节点，主要是可以异步执行。
 * 注册到pipeline内部的所有的异步节点，
 * 会在pipeline结束完成前，全部执行完毕
 * */
template<int Second>
class MyAsyncNode : public CGraph::GAsyncNode {
public:
    CStatus asyncRun() override {
        CGraph::CGRAPH_ECHO("[%s], enter MyAsyncNode run function. Sleep for [%d] second ... ",    \
                            this->getName().c_str(), Second);
        CGRAPH_SLEEP_SECOND(Second)    // sleep 时间由外部设定
        return CStatus();
    }
};

#endif //CGRAPH_MYASYNCNODE_H
