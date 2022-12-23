/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyTemplateDaemon.h
@Time: 2022/12/24 00:14
@Desc: 
***************************/

#ifndef CGRAPH_MYTEMPLATEDAEMON_H
#define CGRAPH_MYTEMPLATEDAEMON_H

#include <string>

#include "../../src/CGraph.h"

template<class ...Args>
class MyTemplateDaemon : public CGraph::GTemplateDaemon<Args...> {
public:
    explicit MyTemplateDaemon(int index) {
        index_ = index;
    }

    void daemonTask(CGraph::GDaemonParamPtr param) override {
        CGraph::CGRAPH_ECHO("----> [MyTemplateDaemon] template input index is [%d]", index_);
    }

private:
    int index_ = 0;
};

#endif //CGRAPH_MYTEMPLATEDAEMON_H
