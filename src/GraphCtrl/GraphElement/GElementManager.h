/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementManager.h
@Time: 2021/6/2 10:33 下午
@Desc:
***************************/

#ifndef CGRAPH_GELEMENTMANAGER_H
#define CGRAPH_GELEMENTMANAGER_H

#include "GElement.h"
#include "GElementSorter.h"
#include "GGroup/GCluster/GCluster.h"
#include "_GEngine/GEngineInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GElementManager : public GElementObject, public GraphManager<GElement> {
protected:
    explicit GElementManager();
    ~GElementManager() override;
    CGRAPH_NO_ALLOWED_COPY(GElementManager);

    CStatus init() override;
    CStatus run() override;
    CStatus destroy() override;

    /**
     * 添加一个元素信息
     * @param element
     * @return
     */
    CStatus add(GElementPtr element) override;

    /**
     * 删除传入元素
     * @param element
     */
    CStatus remove(GElementPtr element) override;

    /**
     * 查找传入元素
     * @param element
     * @return
     */
    CBool find(GElementPtr element) const override;

    /**
     * 清空信息
     * @return
     */
    CStatus clear() final;

    /**
     * 设置调度策略
     * @param type
     * @return
     */
    GElementManager* setScheduleStrategy(int strategy);

    /**
     * 设置图引擎运行模式
     * @param engineType
     * @return
    */
    GElementManager* setEngineType(GEngineType engineType);

    /**
     * 构造执行引擎
     * @param strategy
     * @return
     */
    CStatus initEngine();

private:
    GSortedGElementPtrSet manager_elements_;                    // 保存节点信息的内容
    GEnginePtr engine_ = nullptr;                               // 执行引擎
    GEngineType engine_type_ = GEngineType::STATIC;             // 引擎执行方式

    friend class GPipeline;
    friend class GRegion;
    friend class UAllocator;
};

using GElementManagerPtr = GElementManager *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GELEMENTMANAGER_H
