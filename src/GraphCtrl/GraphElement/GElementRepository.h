/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementRepository.h
@Time: 2023/6/24 16:42
@Desc: 
***************************/

#ifndef CGRAPH_GELEMENTREPOSITORY_H
#define CGRAPH_GELEMENTREPOSITORY_H

#include "GElementObject.h"
#include "GElement.h"

CGRAPH_NAMESPACE_BEGIN

class GElementRepository : public GElementObject {
private:
    /**
     * 插入一个element
     * @param ptr
     * @return
     */
    CVoid insert(GElementPtr ptr);

    /**
     * 查找对应的element
     * @param ptr
     * @return
     */
    CBool find(GElementPtr ptr) const;

    /**
     * 给所有的element，设定线程池
     * @param ptr
     * @return
     */
    GElementRepository* setThreadPool(UThreadPoolPtr ptr);

    /**
     * 准备执行流程
     * @return
     */
    CStatus setup();

    /**
     * 等待所有的element执行结束
     * @return
     * @notice 主要针对异步流程
     */
    CStatus reset();

    /**
     * 设置所有内部的element状态
     * @param state
     * @return
     */
    CStatus pushAllState(const GElementState& state);

    /**
     * 用于判断是否是出于退出状态
     * @return
     */
    CBool isCancelState() const;

    CStatus init() override;

    CStatus destroy() override;

    CStatus run() final;

    ~GElementRepository() override;

private:
    GElementPtrSet elements_;                                    // 用于记录所有的element信息
    GElementState cur_state_ = GElementState::CREATE;            // 当前状态信息
    GElementPtrSet async_elements_;                              // 所有异步执行的逻辑，到后来一次性统一获取执行结果信息

    friend class GPipeline;
    friend class GPerf;
};

using GElementRepositoryPtr = GElementRepository *;
using GElementRepositoryRef = GElementRepository &;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GELEMENTREPOSITORY_H
