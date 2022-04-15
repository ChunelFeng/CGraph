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

CGRAPH_NAMESPACE_BEGIN

class GCluster : public GGroup {
public:
    /** 涉及到与线程池的联动，cluster类无法将构造函数申明成protected类型 */
    explicit GCluster();
    ~GCluster() override;
    GCluster(const GCluster& cluster);
    GCluster& operator=(const GCluster& cluster);

protected:
    /** cluster结构在工程中，被用于自动合并和串行流程。禁止外部修改cluster的逻辑结构 */
    CStatus init() final;
    CStatus destroy() final;
    CStatus run() final;
    CStatus process(bool isMock) final;

    CStatus beforeRun() final;
    CStatus afterRun() final;

    CStatus addElement(GElementPtr element) final;

    /**
     * 获取element个数信息
     * @return
     */
    CSize getElementNum();

    /**
     * 判断是否所有element均执行结束了
     * @return
     */
    CBool isElementsDone();

private:
    GElementPtrArr cluster_elements_;    // 包含的元素信息

    friend class GElementManager;
    friend class GRegion;
    friend class GPipeline;
};

using GClusterPtr = GCluster *;
using GClusterRef = GCluster &;
using GClusterArr = std::vector<GCluster>;
using GClusterArrRef = GClusterArr &;
using ParaWorkedClusterArrs = std::vector<GClusterArr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GCLUSTER_H
