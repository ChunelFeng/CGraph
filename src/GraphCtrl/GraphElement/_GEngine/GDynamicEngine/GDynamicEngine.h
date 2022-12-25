/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GDynamicEngine.h
@Time: 2022/12/16 22:45 下午
@Desc: 
***************************/

#ifndef CGRAPH_GDYNAMICENGINE_H
#define CGRAPH_GDYNAMICENGINE_H

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
     * elment 运行调用
     * @param element
     * @return
    */
    CVoid runElementTask(GElementPtr element);

    /**
     * element 运行
     * @param element
     * @return
     */
    CVoid runElement(GElementPtr element);

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
     * 执行结束节点数量
     * @param
     * @return
    */
    CVoid increaseEnd();

private:
    std::mutex lock_;
    std::condition_variable cv_;
    GSortedGElementPtrSet manager_elements_;                    // 保存节点信息的内容
    CUint end_size_ = 0;                                        // 图结束节点数量
    CUint finish_end_size = 0;                                  // 执行结束节点数量
};

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GDYNAMICENGINE_H