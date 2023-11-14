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
     * 修改内部数据的
     * @param elements
     * @return
     */
    virtual CStatus translate(GElementPtrArr& elements) = 0;

    explicit GMutable();

    ~GMutable() override;

    CStatus addElement(GElementPtr element) final;

    CStatus init() final;

    CStatus run() final;

    CStatus destroy() final;

private:
    GElementManagerPtr manager_ = nullptr;

    friend class GPipeline;
    friend class UAllocator;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMUTABLE_H
