/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyPrintEvent.h
@Time: 2023/1/20 17:57
@Desc: 
***************************/

#ifndef CGRAPH_MYPRINTEVENT_H
#define CGRAPH_MYPRINTEVENT_H

#include "../../src/CGraph.h"

#include "../MyParams/MyParam.h"

class MyPrintEvent : public CGraph::GEvent {
public:
    /**
     * 在本事件被触发的时候，会执行到这里
     * @return
     */
    CVoid trigger(CGraph::GEventParamPtr param) override {
        // 休息100ms，标明跟 notify方法是异步的
        CGRAPH_SLEEP_MILLISECOND(100)
        // event 内部，是打通pipeline内部的GParam获取机制的，可以获取GParam信息
        auto myParam = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1")
        CGraph::CGRAPH_ECHO("----> trigger [%d] times, iValue = [%d]", times_++, myParam->iValue);
    }

private:
    int times_ = 0;
};

#endif //CGRAPH_MYPRINTEVENT_H
