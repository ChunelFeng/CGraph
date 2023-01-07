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
    CStatus setUp(const GSortedGElementPtrSet& elements) override;

    CStatus run() override;

    CStatus afterRunCheck() override;

    /**
     * 动态图运行
     * @param
     * @return
    */
    CVoid asyncRun();

    /**
     * 动态图运行前重置
     * @param
     * @return
    */
    CStatus beforeRun();

    /**
     * element 运行element
     * @param element
     * @return
    */
    CStatus process(GElementPtr element);

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

    /**
     * 查看是否结束
     * @param
     * @return
    */
    CVoid checkFinishState();

private:
    GElementPtrArr total_element_arr_;                          // pipeline中所有的元素信息集合
    GElementPtrArr front_element_arr_;                          // 没有依赖的元素信息
    CSize total_end_size_ = 0;                                  // 图结束节点数量
    CSize finished_end_size_ = 0;                               // 执行结束节点数量
    std::atomic<CSize> run_element_size_;                       // 执行元素的个数，用于后期校验。这里和静态不一样，需要加atomic

    std::mutex lock_;
    std::condition_variable cv_;
};

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GDYNAMICENGINE_H