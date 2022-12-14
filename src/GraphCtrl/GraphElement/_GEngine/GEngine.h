/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEngine.h
@Time: 2022/12/11 16:27
@Desc: 
***************************/

#ifndef CGRAPH_GENGINE_H
#define CGRAPH_GENGINE_H

#include "GEngineObject.h"
#include "../GElementSorter.h"

CGRAPH_NAMESPACE_BEGIN

class GEngine : public GEngineObject {
protected:
    /**
     * 构造函数
     */
    explicit GEngine() {
        thread_pool_ = UThreadPoolSingleton::get();
    }

    /**
     * 将所有注册进入 pipeline的内容，进行分析和解构
     * @param elements
     * @return
     */
    virtual CStatus setUp(const GSortedGElementPtrSet& elements) = 0;

    /**
     * 执行完毕后，确认运行是否正常
     * @return
     */
    virtual CStatus afterRunCheck() = 0;

protected:
    UThreadPoolPtr thread_pool_ { nullptr };                    // 内部执行的线程池
    CUint total_element_size_ = 0;                              // 总的element的数量
    CMSec element_run_ttl_ = CGRAPH_DEFAULT_ELEMENT_RUN_TTL;    // 单个节点最大运行周期
    int schedule_strategy_ = CGRAPH_DEFAULT_TASK_STRATEGY;      // 调度策略

    friend class GElementManager;
    friend class GPipeline;
};

using GEnginePtr = GEngine *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GENGINE_H
