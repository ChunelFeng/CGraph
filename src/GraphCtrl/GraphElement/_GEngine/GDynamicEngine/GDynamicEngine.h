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

#include "../GEngine.h"

CGRAPH_NAMESPACE_BEGIN

class GDynamicEngine : public GEngine {
protected:
    explicit GDynamicEngine() = default;

    CStatus setup(const GSortedGElementPtrSet& elements) override;

    CStatus run() override;

    /**
     * 记录当前 elements 数据信息
     * @param elements
     * @return
     */
    CVoid mark(const GSortedGElementPtrSet& elements);

    /**
     * 分析当前的信息，主要用于区分dag的类型
     * @return
     */
    CVoid analysisDagType(const GSortedGElementPtrSet& elements);

    /**
     * 动态图运行
     * @param
     * @return
    */
    CVoid commonRunAll();

    /**
     * element 运行element
     * @param element
     * @param affinity 是否本地执行
     * @return
    */
    CVoid process(GElementPtr element, CBool affinity);

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
    CVoid fatWait();

    /**
     * 并发的执行所有的element
     * @return
     */
    CVoid parallelRunAll();

    /**
     * 串行的执行所有element
     * @return
     */
    CVoid serialRunAll();

private:
    GElementPtrArr total_element_arr_;                                                   // pipeline中所有的元素信息集合
    GElementPtrArr front_element_arr_;                                                   // 没有依赖的元素信息
    CSize total_end_size_ = 0;                                                           // 图结束节点数量
    CSize finished_end_size_ = 0;                                                        // 执行结束节点数量
    CStatus cur_status_;                                                                 // 当前全局的状态信息
    internal::GEngineDagType dag_type_ = { internal::GEngineDagType::COMMON };           // 当前元素的排布形式

    std::mutex lock_;
    std::condition_variable cv_;
    std::mutex status_lock_;

    friend class UAllocator;
};

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GDYNAMICENGINE_H