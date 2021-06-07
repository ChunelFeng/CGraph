/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GRegion.h
@Time: 2021/6/1 10:14 下午
@Desc: 
***************************/


#ifndef CGRAPH_GREGION_H
#define CGRAPH_GREGION_H

#include "../GElement.h"
#include "../GElementManager.h"
#include "../../GraphThread/GraphThreadPool.h"

/* region 中包含了 cluster 信息 */

class GRegion : public GElement {
protected:
    explicit GRegion();
    ~GRegion() override;

    GRegion(const GRegion& region);
    GRegion& operator=(const GRegion& region);

    CSTATUS init() override;
    CSTATUS deinit() override;
    CSTATUS run() override;
    CSTATUS process(bool isMock) override;

    CSTATUS beforeRun() override;
    CSTATUS afterRun() override;

    CSTATUS setThreadPool(GraphThreadPool* pool);
    CSTATUS addElement(GElementPtr element);
    int getElementNum();
    bool isElementDone();

private:
    GElementManager* manager_;
    GraphThreadPool* thread_pool_;    // 这里需要使用GFlow类的线程池
    GElementPtrArr region_elements_;

    friend class GPipeline;
};

using GRegionPtr = GRegion *;

#endif //CGRAPH_GREGION_H
