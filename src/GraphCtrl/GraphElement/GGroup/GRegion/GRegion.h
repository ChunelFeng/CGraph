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

    /**
     * 判定region运行的时候，是否需要结束。
     * 默认执行一次后，直接结束。可根据需求，进行自定义扩展
     * 如果返回值为true的话，则重新执行run方法，直到返回值为false结束
     * @return
     */
    virtual CBool isHold();

private:
    GElementManagerPtr manager_;

    CGRAPH_NO_ALLOWED_COPY(GRegion)

    friend class GPipeline;
    friend class UAllocator;
};

using GRegionPtr = GRegion *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GREGION_H
