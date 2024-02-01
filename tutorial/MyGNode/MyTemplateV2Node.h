/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyTemplateV2Node.h
@Time: 2024/2/1 21:51
@Desc: 
***************************/

#ifndef CGRAPH_MYTEMPLATEV2NODE_H
#define CGRAPH_MYTEMPLATEV2NODE_H

#include "CGraph.h"

template<int TID>
class MyTemplateV2Node : public CGraph::GNode {
public:
    CStatus run() override {
        CGraph::CGRAPH_ECHO("[MyTemplateV2Node] template id = [%d] ", TID);
        return CStatus();
    }
};

#endif //CGRAPH_MYTEMPLATEV2NODE_H
