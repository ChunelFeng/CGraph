/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphNodeCluster.h
@Time: 2021/5/8 10:59 下午
@Desc: GraphNodeCluster类仅负责保存信息和批量处理信息
***************************/

#ifndef CGRAPH_GRAPHNODECLUSTER_H
#define CGRAPH_GRAPHNODECLUSTER_H

#include "../GraphDefine.h"
#include "GraphNode.h"

class GraphNodeCluster : public CObject {

public:
    explicit GraphNodeCluster() = default;

    ~GraphNodeCluster() override = default;

    CSTATUS init() override;

    CSTATUS deinit() override;

    /**
     * 流程不会进入此函数
     * @return
     */
    CSTATUS run() override;

    /**
     * 批量执行cluster内部节点
     * @return
     */
    CSTATUS multiProcess();

    /**
     * 加入节点信息
     * @param node
     * @return
     */
    CSTATUS addNode(GraphNode* node);

    /**
     * 清空cluster中的内容
     * @return
     */
    void clear();

    [[nodiscard]] int size() const;

    GNodeArr& getNodes();

private:
    GNodeArr node_arr_;
};


#endif //CGRAPH_GRAPHNODECLUSTER_H
