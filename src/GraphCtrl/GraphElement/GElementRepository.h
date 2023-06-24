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
    CBool find(GElementPtr ptr);

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
     * 设置所有内部的element状态
     * @param state
     * @return
     */
    CStatus pushAllState(GElementState state);

    ~GElementRepository() override;

    CStatus run() override;

private:
    GElementPtrSet elements_;        // 用于记录所有的element信息

    friend class GPipeline;
};

using GElementRepositoryPtr = GElementRepository *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GELEMENTREPOSITORY_H
