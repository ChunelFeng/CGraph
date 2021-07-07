/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GRegion.h
@Time: 2021/6/1 10:14 下午
@Desc: 实现多个element，根据依赖关系执行的功能
***************************/


#ifndef CGRAPH_GREGION_H
#define CGRAPH_GREGION_H

#include "../GGroup.h"
#include "../../GElementManager.h"

/* region 中包含了 cluster 信息 */

class GRegion : public GGroup {
protected:
    explicit GRegion();
    ~GRegion() override;

    GRegion(const GRegion& region);
    GRegion& operator=(const GRegion& region);

    CSTATUS init() override;
    CSTATUS deinit() override;
    CSTATUS run() override;
    CSTATUS process(bool isMock) override;

    CSTATUS addElement(GElementPtr element) override;

    CSTATUS setThreadPool(UThreadPoolPtr pool);

private:
    GElementManagerPtr manager_;
    UThreadPoolPtr thread_pool_;    // 这里需要使用GPipeline类的线程池

    friend class GPipeline;
};

using GRegionPtr = GRegion *;

#endif //CGRAPH_GREGION_H
