/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphNode.h
@Time: 2021/4/26 9:29 下午
@Desc: 
***************************/

#ifndef CGRAPH_GRAPHNODE_H
#define CGRAPH_GRAPHNODE_H

#include <list>

#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"

class GraphNode : public CObject {
public:
    GraphNode();

    CSTATUS init() override;

    CSTATUS run() override;

    CSTATUS deinit() override;

    ~GraphNode() override;

public:
    /**
     * 增加对应的依赖
     * @param object
     * @return
     */
    std::list<GraphNode *>& getRunBeforeList();
    CSTATUS addDependNode(GraphNode* node);
    CSTATUS afterRun();

    [[nodiscard]]bool enableRun() const;    // 依赖的内容是否已经执行完毕
    [[nodiscard]]bool isDone() const;

protected:
    CSTATUS addRunBefore(GraphNode* node);
    CSTATUS addDependence(GraphNode* node);

private:
    bool done_;    // 标记被执行结束
    std::list<GraphNode *> dependence_;       // 依赖的节点
    std::list<GraphNode *> run_before_;       // 被依赖的节点
    int left_depend_;    // 当left_cnt_值为0的时候，即可以执行该node信息
};


#endif //CGRAPH_GRAPHNODE_H
