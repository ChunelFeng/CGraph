/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCluster.h
@Time: 2021/6/1 10:14 下午
@Desc: 
***************************/

#ifndef CGRAPH_GCLUSTER_H
#define CGRAPH_GCLUSTER_H

#include <vector>
#include "../GElement.h"

class GCluster : public GElement {
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

    CSTATUS addElement(GElementPtr element);
    int getElementNum();

    /**
     * 判断是否所有elment均执行结束了
     * @return
     */
    bool isElementsDone();

private:
    GElementPtrArr cluster_elements_;    // cluster中包含的元素信息

    friend class GElementManager;
    friend class GRegion;
    friend class GPipeline;
    friend class GraphThreadPool;
};

using GClusterPtr = GCluster *;

#endif //CGRAPH_GCLUSTER_H
