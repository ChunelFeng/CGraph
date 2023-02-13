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
    CStatus addElement(GElementPtr element) final;

    CStatus run() final;

    CStatus process(CBool isMock) final;

    CStatus beforeRun() final;

    CStatus afterRun() final;

    CVoid dump(std::ostream& oss) final;

    /**
     * 获取element个数信息
     * @return
     */
    CSize getElementNum();

    /**
     * 判断是否所有element均执行结束了
     * @return
     */
    CBool isClusterDone();


    friend class GElementManager;
    friend class GRegion;
    friend class GPipeline;
    friend class GStaticEngine;
    friend class GDynamicEngine;
};

using GClusterPtr = GCluster *;
using GClusterRef = GCluster &;
using GClusterArr = std::vector<GCluster>;
using GClusterArrRef = GClusterArr &;
using ParaWorkedClusterArrs = std::vector<GClusterArr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GCLUSTER_H
