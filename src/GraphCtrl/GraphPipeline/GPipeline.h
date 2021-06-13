/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.h
@Time: 2021/6/2 10:15 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPIPELINE_H
#define CGRAPH_GPIPELINE_H

#include <vector>
#include <memory>

#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"
#include "../GraphThread/GraphThreadPool.h"
#include "../GraphElement/GElementManager.h"
#include "../GraphParam/GParamInclude.h"
#include "GPipelineDefine.h"


class GPipeline : public CObject {

public:
    explicit GPipeline();
    ~GPipeline() override;

    /**
     * 初始化pipeline信息
     * @return
     */
    CSTATUS init() override;

    /**
     * 执行pipeline信息
     * @return
     */
    CSTATUS run() override;

    /**
     * 逆初始化pipeline信息
     * @return
     */
    CSTATUS deinit() override;

    /**
     * 一次性执行完成初始化，执行runTimes次，和逆初始化的过程
     * @param runTimes
     * @return
     */
    CSTATUS process(int runTimes = 1);

    /**
     * 根据传入的info信息，创建node节点
     * @tparam T
     * @param info
     * @return
     */
    template<typename T>
    GElementPtr createGNode(const GNodeInfo& info);

    /**
     * 根据传入的信息，创建节点集合（包含cluster和region）
     * @tparam T
     * @param elements
     * @param dependElements
     * @param name
     * @param loop
     * @return
     */
    template<typename T>
    GElementPtr createGNodeS(const GElementPtrArr& elements,
                             const GElementPtrSet& dependElements = std::initializer_list<GElementPtr>(),
                             const std::string& name = "",
                             int loop = 1);

    /**
     * 在图中注册一个Element信息
     * @tparam T
     * @return
     */
    template<typename T>
    CSTATUS registerGElement(GElementPtr* elementRef,
                             const GElementPtrSet& dependElements = std::initializer_list<GElementPtr>(),
                             const std::string& name = "",
                             int loop = 1);

protected:
    /**
     * element元素，添加依赖节点信息
     * @param element
     * @param dependElements
     * @return
     */
    CSTATUS addDependElements(GElementPtr element,
                              const std::set<GElementPtr>& dependElements) const;


private:
    bool is_init_;                                       // 标志位
    GElementManagerPtr element_manager_;                           // 节点管理类（管理所有注册过的element信息）
    GraphThreadPoolPtr thread_pool_;                       // 线程池类
    GElementPtrSet element_repository_;                  // 标记创建的所有节点，最终释放使用
    GParamManagerPtr param_manager_;
};

using GPipelinePtr = GPipeline *;

#include "GPipeline.inl"

#endif //CGRAPH_GPIPELINE_H
