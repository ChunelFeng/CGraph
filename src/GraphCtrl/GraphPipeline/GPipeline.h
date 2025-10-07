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
#include <future>
#include <sstream>

#include "GPipelineObject.h"
#include "_GSchedule/GScheduleInclude.h"
#include "_GStroage/GStorageInclude.h"
#include "../GraphElement/GElementInclude.h"
#include "../GraphDaemon/GDaemonInclude.h"
#include "../GraphEvent/GEventInclude.h"
#include "../GraphStage/GStageInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GPipeline : public GPipelineObject,
                  public CDescInfo {
public:
    CStatus init() override;

    CStatus run() override;

    CStatus destroy() override;

    /**
     * 一次性执行完成初始化，执行runTimes次，和销毁的过程
     * @param runTimes
     * @return
     */
    CStatus process(CSize runTimes = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 异步执行pipeline的run流程
     * @param policy
     * @return
     */
    std::future<CStatus> asyncRun(std::launch policy = std::launch::async);

    /**
     * 异步执行pipeline的全部流程
     * @param runTimes
     * @param policy
     * @return
     */
    std::future<CStatus> asyncProcess(CSize runTimes = CGRAPH_DEFAULT_LOOP_TIMES,
                                      std::launch policy = std::launch::async);

    /**
     * 停止执行流程，多用于异步执行流程中
     * @return
     */
    CStatus cancel();

     /**
      * 暂停当前pipeline的执行，多用于异步执行流程中
      * @return
      */
     CStatus suspend();

     /**
      * 恢复当前pipeline的执行，多用于异步执行流程中
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
     * 查看性能分析
     * @return
     * @notice 将输出的内容，复制到 https://dreampuf.github.io/GraphvizOnline/ 中查看效果，字段解释如下：
     * start : 当前element第一次被执行的时间
     * finish : 当前element最后一次被执行结束的时间
     * per_cost : 当前element单次执行耗时
     * total_cost : 当前element执行的总耗时（仅在element多次执行时展示）
     */
    CStatus perf(std::ostream& oss = std::cout);

    /**
     * 根据传入的info信息，创建node节点
     * @tparam TNode
     * @tparam Args
     * @param info
     * @param args
     * @return
     */
    template<typename TNode, typename ...Args,
            c_enable_if_t<std::is_base_of<GNode, TNode>::value, int> = 0>
    TNode* createGNode(const GNodeInfo &info, Args&&... args);

    /**
     * 根据传入的信息，创建node节点
     * @tparam T
     * @tparam Args
     * @param name
     * @param loop
     * @param dependence
     * @param args
     * @return
     */
    template<typename TNode, typename ...Args,
            c_enable_if_t<std::is_base_of<GNode, TNode>::value, int> = 0>
    TNode* createGNode(const GElementPtrSet& dependence = std::initializer_list<GElementPtr>(),
                       const std::string& name = CGRAPH_EMPTY,
                       CSize loop = CGRAPH_DEFAULT_LOOP_TIMES,
                       Args&&... args);

    /**
     * 根据传入的信息，创建Group信息
     * @tparam T
     * @param elements
     * @param depends
     * @param name
     * @param loop
     * @return
     */
    template<typename TGroup,
            c_enable_if_t<std::is_base_of<GGroup, TGroup>::value, int> = 0>
    TGroup* createGGroup(const GElementPtrArr &elements,
                         const GElementPtrSet &depends = std::initializer_list<GElementPtr>(),
                         const std::string &name = CGRAPH_EMPTY,
                         CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 在图中注册一个 GElement信息
     * 如果注册的是GNode信息，则内部自动生成
     * 如果注册的是GGroup信息，则需外部提前生成，然后注册进来
     * @tparam T
     * @param elementRef
     * @param depends
     * @param name
     * @param loop
     * @return
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<GElement, T>::value, int> = 0>
    CStatus registerGElement(GElementPPtr elementRef,
                             const GElementPtrSet &depends = std::initializer_list<GElementPtr>(),
                             const std::string &name = CGRAPH_EMPTY,
                             CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 在图中注册一个模板Element信息
     * @tparam TNode
     * @tparam Args
     * @param elementRef
     * @param depends
     * @return
     */
    template<typename TNode, typename ...Args,
            c_enable_if_t<std::is_base_of<GTemplateNode<Args ...>, TNode>::value, int> = 0>
    CStatus registerGElement(GTemplateNodePtr<Args ...> *elementRef,
                             const GElementPtrSet &depends,
                             Args... args);

    /**
     * 注册一个 node
     * @tparam T
     * @param depends
     * @param name
     * @param loop
     * @return
     */
    template<typename TNode,
            c_enable_if_t<std::is_base_of<GNode, TNode>::value, int> = 0>
    TNode* registerGNode(const GElementPtrSet &depends = std::initializer_list<GElementPtr>(),
                         const std::string &name = CGRAPH_EMPTY,
                         CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 注册一个 node
     * @tparam TNode
     * @tparam Args
     * @param depends
     * @param args
     * @return
     */
    template<typename TNode, typename ...Args,
            c_enable_if_t<std::is_base_of<GTemplateNode<Args ...>, TNode>::value, int> = 0>
    TNode* registerGNode(const GElementPtrSet &depends,
                         Args... args);

    /**
     * 注册一个节点信息
     * @param nodeRef
     * @param depends
     * @param name
     * @param loop
     * @return
     */
    CStatus registerGNode(GElementPPtr nodeRef,
                          const GElementPtrSet &depends = std::initializer_list<GElementPtr>(),
                          const std::string &name = CGRAPH_EMPTY,
                          CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 注册一个组信息（推荐使用）
     * @param groupRef
     * @param depends
     * @param name
     * @param loop
     * @return
     */
    CStatus registerGGroup(GElementPPtr groupRef,
                           const GElementPtrSet &depends = std::initializer_list<GElementPtr>(),
                           const std::string &name = CGRAPH_EMPTY,
                           CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 注册function类型的内容，模板特化
     * @tparam GFunction
     * @param functionRef
     * @param depends
     * @param name
     * @param loop
     * @return
     */
    template<typename GFunction>
    CStatus registerGElement(GFunctionPPtr functionRef,
                             const GElementPtrSet &depends = std::initializer_list<GElementPtr>(),
                             const std::string &name = CGRAPH_EMPTY,
                             CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 注册fence类型的内容，模板特化
     * @tparam GFence
     * @param fenceRef
     * @param depends
     * @param name
     * @param loop
     * @return
     */
    template<typename GFence>
    CStatus registerGElement(GFencePPtr fenceRef,
                             const GElementPtrSet &depends = std::initializer_list<GElementPtr>(),
                             const std::string &name = CGRAPH_EMPTY,
                             CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 注册GCoordinator类型的内容，模板特化
     * @tparam GCoordinator
     * @tparam SIZE
     * @param coordinatorRef
     * @param depends
     * @param name
     * @param loop
     * @return
     */
    template<typename GCoordinator, CInt SIZE>
    CStatus registerGElement(GCoordinatorPPtr<SIZE> coordinatorRef,
                             const GElementPtrSet &depends = std::initializer_list<GElementPtr>(),
                             const std::string &name = CGRAPH_EMPTY,
                             CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

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
    GPipeline* addGDaemon(CMSec ms, Args&&... args);

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
     * 添加一个阶段
     * @tparam TStage
     * @tparam TParam
     * @param key
     * @param threshold
     * @param param
     * @return
     */
    template<typename TStage, typename TParam = GStageDefaultParam,
            c_enable_if_t<std::is_base_of<GStage, TStage>::value, int> = 0,
            c_enable_if_t<std::is_base_of<GStageParam, TParam>::value, int> = 0>
    GPipeline* addGStage(const std::string& key, CInt threshold, TParam* param = nullptr);

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
     * @return
     * @notice 暂时仅支持dag中所有内容均为 node的情况下计算。返回的值，是理论最大线程数，不是最优值
     */
    CSize getMaxPara();

    /**
     * 针对图结构，做多余边剪裁
     * @return 返回值表示裁剪了多少条信息
     */
    CSize trim();

    /**
     * 将符合串行执行条件的pipeline，设定为串行执行的模式。可以大幅度提升运行性能。
     * @return
     * @notice 如果pipeline不可以设定的话，通过返回值提示对应信息
     */
    CStatus makeSerial();

    /**
     * 获取当前的执行状态
     * @return
     */
    GPipelineState getCurState() const;

    /**
     * 判断两个element，是否有依赖关系
     * @param fst
     * @param snd
     * @return
     * @notice 如果返回 true，则表示不可能并发执行；否则表示可能并发执行
     */
    CBool checkSeparate(GElementPtr fst, GElementPtr snd) const;

    /**
     * 存储当前 pipeline 信息到本地
     * @param path
     * @return
     */
    CStatus save(const std::string& path);

    /**
     * 从本地文件中加载 pipeline 信息
     * @param path
     * @return
     */
    CStatus load(const std::string& path);

    /**
     * 注册GParam 交互类集合
     * @return
     */
    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER

protected:
    explicit GPipeline();
    ~GPipeline() override;

private:
    /**
     * 初始化环境信息，包括线程池 等
     * @return
     */
    CStatus initEnv();

    /**
     * 内部真实一个 element 信息
     * @param element
     * @param depends
     * @param name
     * @param loop
     * @return
     */
    CStatus innerRegister(GElementPtr element, const GElementPtrSet &depends,
                          const std::string &name, CSize loop);

    /** 不允许外部赋值和构造 */
    CGRAPH_NO_ALLOWED_COPY(GPipeline)

private:
    GElementManagerPtr element_manager_ = nullptr;              // 节点管理类（管理所有注册过的element信息）
    GParamManagerPtr param_manager_ = nullptr;                  // 参数管理类
    GDaemonManagerPtr daemon_manager_ = nullptr;                // 守护管理类
    GEventManagerPtr event_manager_ = nullptr;                  // 事件管理类
    GStageManagerPtr stage_manager_ = nullptr;                  // 阶段管理类

    GSchedule schedule_;                                        // 调度管理类
    GElementRepository repository_;                             // 记录创建的所有element的仓库

    friend class GPipelineFactory;
    friend class CAllocator;
    friend class GPerf;
    friend class GStorage;

public:
    CStatus __registerGElement_4py(GElementPtr element, const GElementPtrSet &depends,
                                   const std::string &name, CSize loop);
    GPipeline* __addGEvent_4py(GEventPtr event, const std::string& key);
    GPipeline* __addGDaemon_4py(GDaemonPtr daemon, CMSec ms);
    GPipeline* __addGStage_4py(GStagePtr stage, const std::string& key, CInt threshold);
    std::string __dump_4py();
    std::string __perf_4py();
};

using GPipelinePtr = GPipeline *;
using GPipelinePtrList = std::list<GPipelinePtr>;

CGRAPH_NAMESPACE_END

#include "GPipeline.inl"

#endif //CGRAPH_GPIPELINE_H
