/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEngine.h
@Time: 2022/12/11 16:27
@Desc: 
***************************/

#ifndef CGRAPH_GENGINE_H
#define CGRAPH_GENGINE_H

#include <vector>
#include <queue>

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

    /**
     * 计算当前elements的 拓扑排序信息
     * @param elements
     * @return
     */
    static GElementPtrArr getTopo(const GSortedGElementPtrSet& elements) {
        GElementPtrArr result;
        std::queue<GElementPtr> readyQueue;
        for (auto* element : elements) {
            element->left_depend_ = element->dependence_.size();
            if (0 == element->left_depend_) {
                readyQueue.push(element);
            }
        }

        while(!readyQueue.empty()) {
            auto* cur = readyQueue.front();
            readyQueue.pop();
            result.push_back(cur);

            for (auto* element : cur->run_before_) {
                if (0 == --element->left_depend_) {
                    readyQueue.push(element);
                }
            }
        }

        for (auto element : elements) {
            // 计算技术之后，需要恢复一下 depend的信息，以免引入误差
            element->left_depend_ = element->dependence_.size();
        }
        return result;
    }

    /**
    * 判断是否是dag的逻辑
    * @param elements
    * @return
    */
    static CBool isDag(const GSortedGElementPtrSet& elements) {
        const auto& result = getTopo(elements);
        return result.size() == elements.size();
    }

protected:
    UThreadPoolPtr thread_pool_ { nullptr };                    // 内部执行的线程池
    CSize linked_size_ = 0;                                     // 标记有多少个element，是 linkable 的数据

    friend class GElementManager;
    friend class GPipeline;
    friend class GMutable;
};

using GEnginePtr = GEngine *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GENGINE_H
