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

CGRAPH_NAMESPACE_BEGIN

/** region 中包含了 cluster 的所有功能 */
class GRegion : public GGroup {
protected:
    explicit GRegion();
    ~GRegion() override;

    CStatus init() final;
    CStatus run() final;
    CStatus destroy() final;

    CStatus addElement(GElementPtr element) final;

private:
    GElementManagerPtr manager_;    // region 内部通过 manager来管理其中的 element 信息

    CGRAPH_NO_ALLOWED_COPY(GRegion)


    friend class GPipeline;
    friend class UAllocator;
};

using GRegionPtr = GRegion *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GREGION_H
