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
    GCluster(const GCluster& cluster);
    GCluster& operator=(const GCluster& cluster);

protected:
    explicit GCluster();

    /**
     * 获取element个数信息
     * @return
     */
    CSize getElementNum();

    /**
     * 获取绑定信息
     * @return
     */
    CIndex getBindingIndex() override;

private:
    /**
     * 线程池中的运行函数，依次执行beforeRun，run和afterRun方法，
     * 其中有任何返回值问题，则直接返回
     * @param isMock 是否真实执行run方法。默认执行的
     * @return
     */
    CStatus process(CBool isMock);

    CStatus run() final;

    CStatus addElement(GElementPtr element) final;

    CVoid beforeRun() final;

    CVoid afterRun() final;

    CVoid dump(std::ostream& oss) final;

    /**
     * 判断是否所有element均执行结束了
     * @return
     */
    CBool isDone();

    friend class GPipeline;
    friend class GStaticEngine;
    friend class GDynamicEngine;
    friend class UAllocator;
};

using GClusterPtr = GCluster *;
using GClusterRef = GCluster &;
using GClusterArr = std::vector<GCluster>;
using GClusterArrRef = GClusterArr &;
using ParaWorkedClusterArrs = std::vector<GClusterArr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GCLUSTER_H
