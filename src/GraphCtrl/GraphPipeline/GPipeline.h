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
#include <list>

#include "../GraphElement/GElementInclude.h"
#include "../GraphDaemon/GDaemonInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GPipeline : public GraphObject {
public:
    /**
     * 初始化pipeline信息
     * @return
     */
    CStatus init() override;

    /**
     * 执行pipeline信息
     * @return
     */
    CStatus run() override;

    /**
     * 逆初始化pipeline信息
     * @return
     */
    CStatus destroy() override;

    /**
     * 一次性执行完成初始化，执行runTimes次，和销毁的过程
     * @param runTimes
     * @return
     */
    CStatus process(CSize runTimes = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 根据传入的info信息，创建node节点
     * @tparam T
     * @tparam level
     * @param info
     * @return
     */
    template<typename T, CLevel level = CGRAPH_DEFAULT_ELEMENT_LEVEL,
            std::enable_if_t<std::is_base_of<GNode, T>::value, int> = 0>
    GNodePtr createGNode(const GNodeInfo &info);

    /**
     * 根据传入的信息，创建Group信息
     * @tparam T
     * @tparam level
     * @param elements
     * @param dependElements
     * @param name
     * @param loop
     * @return
     */
    template<typename T, CLevel level = CGRAPH_DEFAULT_ELEMENT_LEVEL,
            std::enable_if_t<std::is_base_of<GGroup, T>::value, int> = 0>
    GGroupPtr createGGroup(const GElementPtrArr &elements,
                           const GElementPtrSet &dependElements = std::initializer_list<GElementPtr>(),
                           const std::string &name = CGRAPH_EMPTY,
                           CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 在图中注册一个Element信息
     * 如果注册的是GNode信息，则内部自动生成
     * 如果注册的是GGroup信息，则需外部提前生成，然后注册进来
     * @tparam T
     * @tparam level
     * @param elementRef
     * @param dependElements
     * @param name
     * @param loop
     * @return
     */
    template<typename T, CLevel level = CGRAPH_DEFAULT_ELEMENT_LEVEL,
            std::enable_if_t<std::is_base_of<GElement, T>::value, int> = 0>
    CStatus registerGElement(GElementPtr *elementRef,
                             const GElementPtrSet &dependElements = std::initializer_list<GElementPtr>(),
                             const std::string &name = CGRAPH_EMPTY,
                             CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 注册function类型的内容，模板特化
     * @tparam GFunction
     * @tparam level
     * @param functionRef
     * @param dependElements
     * @param name
     * @param loop
     * @return
     */
    template<typename GFunction, CLevel level = CGRAPH_DEFAULT_ELEMENT_LEVEL>
    CStatus registerGElement(GFunctionPtr *functionRef,
                             const GElementPtrSet &dependElements = std::initializer_list<GElementPtr>(),
                             const std::string &name = CGRAPH_EMPTY,
                             CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 在图中注册一个模板Element信息
     * @tparam TNode
     * @tparam Args
     * @param elementRef
     * @param dependElements
     * @return
     */
    template<typename TNode, typename ...Args,
            std::enable_if_t<std::is_base_of<GTemplateNode<Args ...>, TNode>::value, int> = 0>
    CStatus registerGElement(GTemplateNodePtr<Args ...> *elementRef,
                             const GElementPtrSet &dependElements = std::initializer_list<GElementPtr>(),
                             Args... args);

    /**
     * 添加参数，pipeline中所有节点共享此参数
     * @tparam T
     * @param key
     * @return
     */
    template<typename T, std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    CStatus createGParam(const std::string& key);

    /**
     * 添加切面
     * @tparam T
     * @param elements
     * @return
     */
    template<typename TAspect, typename TParam = GAspectDefaultParam,
            std::enable_if_t<std::is_base_of<GAspect, TAspect>::value, int> = 0,
            std::enable_if_t<std::is_base_of<GAspectParam, TParam>::value, int> = 0>
    GPipeline* addGAspect(const GElementPtrSet& elements = std::initializer_list<GElementPtr>(),
                          TParam* param = nullptr);

    /**
     * 添加守护
     * @tparam T
     * @param ms 设置定时间隔信息，单位是ms
     * @param param 传入参数信息
     * @return
     */
    template<typename TDaemon, typename DParam = GDaemonDefaultParam,
            std::enable_if_t<std::is_base_of<GDaemon, TDaemon>::value, int> = 0,
            std::enable_if_t<std::is_base_of<GDaemonParam, DParam>::value, int> = 0>
    GPipeline* addGDaemon(CMSec ms, DParam* param = nullptr);

    /**
     * 添加模板类型守护
     * @tparam TAspect
     * @tparam Args
     * @param ms
     * @param args
     * @return
     */
    template<typename TDaemon, typename ...Args,
            std::enable_if_t<std::is_base_of<GTemplateDaemon<Args...>, TDaemon>::value, int> = 0>
    GPipeline* addGDaemon(CMSec ms, Args... args);

    /**
     * 设置执行的最大时间周期，单位为毫秒
     * @param ttl
     * @return
     * @notice beta接口
     */
    GPipeline* setGElementRunTtl(CMSec ttl);

    /**
     * 设置引擎策略
     * @param type
     * @return
     */
    GPipeline* setGEngineType(GEngineType type);

protected:
    explicit GPipeline();
    ~GPipeline() override;

    /** 不允许外部赋值和构造 */
    CGRAPH_NO_ALLOWED_COPY(GPipeline)

private:
    CBOOL is_init_ = false;                                     // 初始化标志位
    GElementManagerPtr element_manager_;                        // 节点管理类（管理所有注册过的element信息）
    GElementPtrSet element_repository_;                         // 标记创建的所有节点，最终释放使用
    GParamManagerPtr param_manager_;                            // 参数管理类
    UThreadPoolPtr thread_pool_;                                // 线程池类
    GDaemonManagerPtr daemon_manager_;                          // 守护管理类

    friend class GPipelineFactory;
    friend class UAllocator;
};

using GPipelinePtr = GPipeline *;
using GPipelinePtrList = std::list<GPipelinePtr>;

CGRAPH_NAMESPACE_END

#include "GPipeline.inl"

#endif //CGRAPH_GPIPELINE_H
