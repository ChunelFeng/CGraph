/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MySome.h
@Time: 2026/6/24 23:23
@Desc: 
***************************/

#ifndef CGRAPH_MYSOME_H
#define CGRAPH_MYSOME_H

#include "CGraph.h"

class MySome : public CGraph::GSome {
protected:
    CSize getWaitNum() override {
        /**
         * 执行完 1个之后，当前的 GSome 就继续往后执行
         * 其余的 element，会在 pipeline 结束之前，执行完成。
         */
        return 1;
    }
};

#endif //CGRAPH_MYSOME_H
