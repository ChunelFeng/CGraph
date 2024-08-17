/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEngine.h
@Time: 2022/12/11 16:27
@Desc: 
***************************/

#ifndef CGRAPH_GENGINE_H
#define CGRAPH_GENGINE_H

#include "GEngineDefine.h"
#include "../GElementObject.h"
#include "../GElementSorter.h"

CGRAPH_NAMESPACE_BEGIN

class GEngine : public GElementObject {
protected:
    explicit GEngine() = default;

    /**
     * 将所有注册进入 pipeline的内容，进行分析和解构
     * @param elements
     * @return
     */
    virtual CStatus setup(const GSortedGElementPtrSet& elements) = 0;

    /**
     * 执行完毕后，确认运行是否正常
     * @return
     */
    virtual CStatus afterRunCheck() {
        CGRAPH_EMPTY_FUNCTION
    }

    /**
     * 计算出来最终计算的index值
     * @param element
     * @return
     */
    CIndex calcIndex(GElementCPtr element) const {
        /**
         * 如果没有设定绑定线程的话，就用默认调度策略
         * 否则的话，会走绑定的thread。
         * 如果设定的 binding_index_ >= thread 总数，会在 threadpool 层做统一判定
         */
        return element->isDefaultBinding()
               ? schedule_strategy_ : element->binding_index_;
    }

    /**
     * 分析所有的可以设置 linkable 的数据
     * @param elements
     * @return
     */
    CVoid link(const GSortedGElementPtrSet& elements) {
        /**
         * 认定图可以连通的判定条件：
         * 1，当前元素仅有一个后继
         * 2，当前元素的唯一后继，仅有一个前驱
         * 3，前后元素绑定机制是一样的
         */
        linked_size_ = 0;
        for (GElementPtr element : elements) {
            element->shape_ = internal::GElementShape::NORMAL;
            if (1 == element->run_before_.size()
                && 1 == (*element->run_before_.begin())->dependence_.size()
                && element->binding_index_ == (*(element->run_before_.begin()))->binding_index_) {
                element->shape_ = internal::GElementShape::LINKABLE;
                linked_size_++;
            }
        }
    }


protected:
    UThreadPoolPtr thread_pool_ { nullptr };                    // 内部执行的线程池
    int schedule_strategy_ = CGRAPH_DEFAULT_TASK_STRATEGY;      // 调度策略
    CSize linked_size_ = 0;                                     // 标记有多少个element，是 linkable 的数据

    friend class GElementManager;
    friend class GPipeline;
    friend class GMutable;
};

using GEnginePtr = GEngine *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GENGINE_H
