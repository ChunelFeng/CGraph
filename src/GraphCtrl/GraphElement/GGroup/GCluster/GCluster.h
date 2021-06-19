/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCluster.h
@Time: 2021/6/1 10:14 下午
@Desc: 实现多个element，线性执行的功能
***************************/

#ifndef CGRAPH_GCLUSTER_H
#define CGRAPH_GCLUSTER_H

#include <vector>
#include "../GGroup.h"

class GCluster : public GGroup {
public:
    /* 涉及到与线程池的联动，cluster类无法将构造函数申明成protected类型 */
    explicit GCluster();
    ~GCluster() override;
    GCluster(const GCluster& cluster);
    GCluster& operator=(const GCluster& cluster);

protected:
    CSTATUS init() override;
    CSTATUS deinit() override;
    CSTATUS run() override;
    CSTATUS process(bool isMock) override;

    CSTATUS beforeRun() override;
    CSTATUS afterRun() override;

    CSTATUS addElement(GElementPtr element) override;
    int getElementNum();

    /**
     * 判断是否所有elment均执行结束了
     * @return
     */
    bool isElementsDone();

private:
    GElementPtrArr cluster_elements_;    // 包含的元素信息

    friend class GElementManager;
    friend class GRegion;
    friend class GPipeline;
    friend class GraphThreadPool;
};

using GClusterPtr = GCluster *;
using GClusterArr = std::vector<GCluster>;
using ParaWorkedClusterArrs = std::vector<GClusterArr>;


#endif //CGRAPH_GCLUSTER_H
