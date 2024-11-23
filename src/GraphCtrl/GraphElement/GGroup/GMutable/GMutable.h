/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMutable.h
@Time: 2023/11/13 22:21
@Desc: 
***************************/

#ifndef CGRAPH_GMUTABLE_H
#define CGRAPH_GMUTABLE_H

#include "../GGroup.h"
#include "../../GElementManager.h"

CGRAPH_NAMESPACE_BEGIN

class GMutable : public GGroup {
protected:
    /**
     * 重新设定内部数据的结构信息
     * @param elements
     * @return
     */
    virtual CStatus reshape(GElementPtrArr& elements) = 0;

    explicit GMutable();

    ~GMutable() override;

private:
    CStatus init() final;

    CStatus run() final;

    CStatus destroy() final;

    CBool isSerializable() const override;

    /**
     * 将数据进行恢复
     * @return
     */
    CVoid setup();

private:
    GElementManagerPtr manager_ = nullptr;

    friend class GPipeline;
    friend class CAllocator;
};

using GMutablePtr = GMutable *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMUTABLE_H
