/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphNodeManager.h
@Time: 2021/5/6 10:38 下午
@Desc: 
***************************/


#ifndef CGRAPH_GRAPHNODEMANAGER_H
#define CGRAPH_GRAPHNODEMANAGER_H

#include <set>
#include "../../CObject/CObject.h"
#include "GraphNode.h"
#include "../GraphDefine.h"

class GraphNodeManager : public CObject {
public:
    explicit GraphNodeManager();
    virtual ~GraphNodeManager();
    CSTATUS init() override;
    CSTATUS deinit() override;
    CSTATUS run() override;

    CSTATUS insertNode(GraphNode* node);
    CSTATUS multiProcess(const GNodeArr& nodes);
    bool hasNode(GraphNode* node) const;
    void deleteNode(GraphNode* node);

private:
    std::set<GraphNode *> graph_nodes_;    // 保存节点信息的内容

private:
    friend class Graphic;
};


#endif //CGRAPH_GRAPHNODEMANAGER_H
