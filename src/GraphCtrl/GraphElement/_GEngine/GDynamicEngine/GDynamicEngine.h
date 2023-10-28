/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GDynamicEngine.h
@Time: 2022/12/16 22:45 下午
@Desc: 
***************************/

#ifndef CGRAPH_GDYNAMICENGINE_H
#define CGRAPH_GDYNAMICENGINE_H

#include <mutex>
#include <atomic>

#include "../GEngine.h"

CGRAPH_NAMESPACE_BEGIN

class GDynamicEngine : public GEngine {
protected:
    explicit GDynamicEngine() = default;

    CStatus setup(const GSortedGElementPtrSet& elements) override;

    CStatus run() override;

    CStatus afterRunCheck() override;

    /**
     * 动态图运行
     * @param
     * @return
    */
    CVoid asyncRunAndWait();

    /**
     * 动态图运行前重置
     * @param
     * @return
    */
    CVoid beforeRun();

    /**
     * element 运行element
     * @param element
     * @param affinity 是否本地执行
     * @return
    */
    CStatus process(GElementPtr element, CBool affinity);

    /**
     * element 运行完成处理
     * @param element
     * @return
     */
    CVoid afterElementRun(GElementPtr element);

    /**
     * 动态图运行等待
     * @param
     * @return
    */
    CVoid wait();

private:
    GElementPtrArr total_element_arr_;                          // pipeline中所有的元素信息集合
    GElementPtrArr front_element_arr_;                          // 没有依赖的元素信息
    CSize total_end_size_ = 0;                                  // 图结束节点数量
    CSize finished_end_size_ = 0;                               // 执行结束节点数量
    std::atomic<CSize> run_element_size_ { 0 };              // 执行元素的个数，用于后期校验。这里和静态不一样，需要加atomic
    CStatus cur_status_;                                        // 当前全局的状态信息

    std::mutex lock_;
    std::condition_variable cv_;

    friend class UAllocator;
};

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GDYNAMICENGINE_H