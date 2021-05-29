/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphLoop.h
@Time: 2021/5/28 10:23 下午
@Desc: 
***************************/

#ifndef CGRAPH_GRAPHNODELOOPREGION_H
#define CGRAPH_GRAPHNODELOOPREGION_H

#include <vector>
#include "../../CObject/CObject.h"
#include "GraphNodeCluster.h"
#include "../../UtilsCtrl/UtilsInclude.h"

/* cluster队列 */
using ClusterArr = std::vector<GraphNodeCluster>;
using ParaWorkedClusterArr = std::vector<ClusterArr>;

class GraphNodeLoopRegion : public CObject {

public:
    GraphNodeLoopRegion();
    ~GraphNodeLoopRegion() override;

    /**
     * 实现拷贝构造函数
     * @param cluster
     */
    GraphNodeLoopRegion(const GraphNodeLoopRegion& loopRegion);

    /**
     * 赋值构造函数
     * @param cluster
     * @return
     */
    GraphNodeLoopRegion& operator=(const GraphNodeLoopRegion& loopRegion);

    CSTATUS run() override;

    void clear();

protected:
    CSTATUS addClusterArr(ClusterArr& clusterArr, bool isHead);

private:
    int loop_;
    ParaWorkedClusterArr para_worked_clusters_;

    friend class Graphic;
};


#endif //CGRAPH_GRAPHLOOP_H
