/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPipeline.h
@Time: 2021/6/2 10:15 下午
@Desc:
***************************/

#ifndef CGRAPH_GPIPELINE_H
#define CGRAPH_GPIPELINE_H

#include <vector>
#include <memory>
#include <list>
#include <thread>
#include <sstream>

#include "GPipelineObject.h"
#include "_GSchedule/GScheduleInclude.h"
#include "../GraphElement/GElementInclude.h"
#include "../GraphDaemon/GDaemonInclude.h"
#include "../GraphEvent/GEventInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GPipeline : public GPipelineObject,
                  public CDescInfo {
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
     * 异步执行pipeline的run流程
     * @return
     */
    std::future<CStatus> asyncRun();

    /**
     * 异步执行pipeline的全部流程
     * @param runTimes
     * @return
     */
    std::future<CStatus> asyncProcess(CSize runTimes = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 停止执行流程，多用于异步执行流程中
     * @return
     */
    CStatus cancel();

     /**
      * 暂停当前pipeline的执行
      * @return
      */
     CStatus yield();

     /**
      * 恢复当前pipeline的执行
      * @return
      */
     CStatus resume();

    /**
     * 生成图可视化 graphviz 信息
     * @param oss
     * @return
     * @notice 将输出的内容，复制到 https://dreampuf.github.io/GraphvizOnline/ 中查看效果
    */
    CStatus dump(std::ostream& oss = std::cout);

    /**
     * 根据传入的info信息，创建node节点
     * @tparam T
     * @param info
     * @return
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<GNode, T>::value, int> = 0>
    GNodePtr createGNode(const GNodeInfo &info);

    /**
     * 根据传入的信息，创建Group信息
     * @tparam T
     * @param elements
     * @param dependElements
     * @param name
     * @param loop
     * @return
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<GGroup, T>::value, int> = 0>
    GGroupPtr createGGroup(const GElementPtrArr &elements,
                           const GElementPtrSet &dependElements = std::initializer_list<GElementPtr>(),
                           const std::string &name = CGRAPH_EMPTY,
                           CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 在图中注册一个Element信息
     * 如果注册的是GNode信息，则内部自动生成
     * 如果注册的是GGroup信息，则需外部提前生成，然后注册进来
     * @tparam T
     * @param elementRef
     * @param dependElements
     * @param name
     * @param loop
     * @return
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<GElement, T>::value, int> = 0>
    CStatus registerGElement(GElementPtr *elementRef,
                             const GElementPtrSet &dependElements = std::initializer_list<GElementPtr>(),
                             const std::string &name = CGRAPH_EMPTY,
                             CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 注册function类型的内容，模板特化
     * @tparam GFunction
     * @param functionRef
     * @param dependElements
     * @param name
     * @param loop
     * @return
     */
    template<typename GFunction>
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
     * @notice Android平台NDK R16B 环境不支持 std::initializer_list<> 和 Args... 混用。参考链接：https://github.com/ChunelFeng/CGraph/pull/116
     */
    template<typename TNode, typename ...Args,
            c_enable_if_t<std::is_base_of<GTemplateNode<Args ...>, TNode>::value, int> = 0>
    CStatus registerGElement(GTemplateNodePtr<Args ...> *elementRef,
                                  #if defined(__ANDROID__)
                             const GElementPtrSet &dependElements,
                                  #else
                             const GElementPtrSet &dependElements = std::initializer_list<GElementPtr>(),
                                  #endif
                             Args... args);

    /**
     * 添加切面
     * @tparam TAspect
     * @tparam TParam
     * @param elements
     * @param param
     * @return
     */
    template<typename TAspect, typename TParam = GAspectDefaultParam,
            c_enable_if_t<std::is_base_of<GAspect, TAspect>::value, int> = 0,
            c_enable_if_t<std::is_base_of<GAspectParam, TParam>::value, int> = 0>
    GPipeline* addGAspect(const GElementPtrSet& elements = std::initializer_list<GElementPtr>(),
                          TParam* param = nullptr);

    /**
     * 添加守护
     * @tparam TDaemon
     * @tparam TParam
     * @param ms
     * @param param
     * @return
     */
    template<typename TDaemon, typename TParam = GDaemonDefaultParam,
            c_enable_if_t<std::is_base_of<GDaemon, TDaemon>::value, int> = 0,
            c_enable_if_t<std::is_base_of<GDaemonParam, TParam>::value, int> = 0>
    GPipeline* addGDaemon(CMSec ms, TParam* param = nullptr);

    /**
     * 添加模板类型守护
     * @tparam TAspect
     * @tparam Args
     * @param ms
     * @param args
     * @return
     */
    template<typename TDaemon, typename ...Args,
            c_enable_if_t<std::is_base_of<GTemplateDaemon<Args...>, TDaemon>::value, int> = 0>
    GPipeline* addGDaemon(CMSec ms, Args... args);

    /**
     * 添加一个事件
     * @tparam TEvent
     * @tparam TParam
     * @param key
     * @param param
     * @return
     */
    template<typename TEvent, typename TParam = GEventDefaultParam,
            c_enable_if_t<std::is_base_of<GEvent, TEvent>::value, int> = 0,
            c_enable_if_t<std::is_base_of<GEventParam, TParam>::value, int> = 0>
    GPipeline* addGEvent(const std::string& key, TParam* param = nullptr);

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

    /**
     * 设置本pipeline内部线程池相关信息
     * @param config
     * @return
     */
    GPipeline* setUniqueThreadPoolConfig(const UThreadPoolConfig& config);

    /**
     * 设置共享的线程池
     * @param ptr
     * @return
     * @notice 如果传入 nullptr 的话，则默认使用本地独占线程池
     */
    GPipeline* setSharedThreadPool(UThreadPoolPtr ptr);

    /**
     * 获取最大并发度
     * @param size
     * @return
     * @notice 暂时仅支持dag中所有内容均为 node的情况下计算。返回的值，是理论最大线程数，不是最优值
     */
    CStatus calcMaxPara(CSize& size);

    /**
     * 注册GParam 交互类集合
     * @return
     */
    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER

protected:
    explicit GPipeline();
    ~GPipeline() override;

    /**
     * 初始化调度信息，包括线程池
     * @return
     */
    CStatus initSchedule();

    /**
     * 准备执行流程
     * @return
     */
    CVoid prepare();

    /**
     * 设置所有内部的element状态
     * @param state
     * @return
     */
    CStatus pushAllState(GElementState state);

    /** 不允许外部赋值和构造 */
    CGRAPH_NO_ALLOWED_COPY(GPipeline)

private:
    GElementManagerPtr element_manager_ = nullptr;              // 节点管理类（管理所有注册过的element信息）
    GParamManagerPtr param_manager_ = nullptr;                  // 参数管理类
    GDaemonManagerPtr daemon_manager_ = nullptr;                // 守护管理类
    GEventManagerPtr event_manager_ = nullptr;                  // 事件管理类

    GSchedule schedule_;                                        // 调度管理类
    GElementPtrSet element_repository_;                         // 标记创建的所有节点，最终释放使用

    friend class GPipelineFactory;
    friend class UAllocator;
};

using GPipelinePtr = GPipeline *;
using GPipelinePtrList = std::list<GPipelinePtr>;

CGRAPH_NAMESPACE_END

#include "GPipeline.inl"

#endif //CGRAPH_GPIPELINE_H
