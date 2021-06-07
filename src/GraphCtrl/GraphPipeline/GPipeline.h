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
#include "GPipelineDefine.h"


class GPipeline : public CObject {

public:
    explicit GPipeline();
    ~GPipeline() override;

    CSTATUS init() override;
    CSTATUS run() override;
    CSTATUS deinit() override;

    /**
     * 根据传入的info信息，创建GNode
     * @tparam T
     * @param info
     * @return
     */
    template<typename T>
    GNodePtr createGNode(const GNodeInfo& info);

    /**
     * 创建一个 cluster 信息
     * @param elements
     * @return
     */
    GClusterPtr createGCluster(const GElementPtrArr& elements,
                               const GElementPtrSet& dependElements = std::initializer_list<GElementPtr>(),
                               const std::string& name = "",
                               int loop = 1);

    /**
     * 创建一个 region 信息
     * region与cluster的区别，在于内部elements管理的方式不同
     * cluster是线性的，region是有依赖关系的
     * @param elements
     * @return
     */
    GRegionPtr createGRegion(const GElementPtrArr& elements,
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
    GElementManager* manager_;                           // 节点管理类（管理所有注册过的element信息）
    GraphThreadPool* thread_pool_;                       // 线程池类
    GElementPtrSet element_repository_;                  // 标记创建的所有节点，最终释放使用
};

using GPipelinePtr = GPipeline *;

#include "GPipeline.inl"

#endif //CGRAPH_GPIPELINE_H
