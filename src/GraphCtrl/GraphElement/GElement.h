/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElement.h
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

#ifndef CGRAPH_GELEMENT_H
#define CGRAPH_GELEMENT_H

#include <set>
#include <vector>
#include <string>
#include <atomic>
#include <algorithm>

#include "GElementDefine.h"
#include "GElementObject.h"

CGRAPH_NAMESPACE_BEGIN

class GElement : public GElementObject,
                 public CDescInfo {
public:
    /**
     * 实现添加切面的逻辑
     * @tparam TAspect
     * @tparam TParam
     * @param param
     * @return
     */
    template<typename TAspect, typename TParam = GAspectDefaultParam,
            c_enable_if_t<std::is_base_of<GAspect, TAspect>::value, int> = 0,
            c_enable_if_t<std::is_base_of<GAspectParam, TParam>::value, int> = 0>
    GElement* addGAspect(TParam* param = nullptr);

    /**
     * 实现添加模板切面的逻辑
     * @tparam TAspect
     * @tparam Args
     * @return
     */
    template<typename TAspect, typename ...Args,
            c_enable_if_t<std::is_base_of<GTemplateAspect<Args...>, TAspect>::value, int> = 0>
    GElement* addGAspect(Args... args);

    /**
     * 添加当前element内部参数
     * @tparam T
     * @param key
     * @param param
     * @return
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<GElementParam, T>::value, int> = 0>
    GElement* addEParam(const std::string& key, T* param);

    /**
     * 添加依赖节点信息
     * @param elements
     * @return
     */
    CStatus addDependGElements(const std::set<GElement *>& elements);

    /**
     * 设置name信息
     * @param name
     * @return
     */
    GElement* setName(const std::string& name) override;

    /**
     * 设置循环次数
     * @param loop
     * @return
     */
    GElement* setLoop(CSize loop);

    /**
     * 设置level信息，用于控制init和destroy方法的执行顺序
     * level值越低，函数越先执行
     * @param level
     * @return
     */
    GElement* setLevel(CLevel level);

protected:
    /**
     * 构造函数
     */
    explicit GElement();

    /**
     * 析构函数
     */
    ~GElement() override;

    /**
     * run方法执行之前的执行函数
     * @return
     */
    virtual CStatus beforeRun();

    /**
     * run方法执行之后的执行函数
     * @return
     */
    virtual CStatus afterRun();

    /**
     * 是否持续进行
     * 默认为false，表示执行且仅执行一次
     * @return
     */
    virtual CBool isHold();

    /**
     * 崩溃流程处理
     * @param ex
     * @return
     */
    virtual CStatus crashed(const CException& ex);

    /**
     * 判定element是否可以运行
     * 可执行的条件为：自身未被执行且依赖节点全部被执行
     * @return
     */
    [[nodiscard]] CBool isRunnable() const;

    /**
     * 判定node是否可以和前面节点一起执行
     * @return
     */
    [[nodiscard]] CBool isLinkable() const;

    /**
     * 线程池中的运行函数，依次执行beforeRun，run和afterRun方法，
     * 其中有任何返回值问题，则直接返回
     * @param isMock 是否真实执行run方法。默认执行的
     * @return
     */
    virtual CStatus process(bool isMock);

    /**
     * 执行切面逻辑
     * @param aspectType
     * @param curStatus
     * @return
     */
    CStatus doAspect(const GAspectType& aspectType,
                     const CStatus& curStatus = CStatus());

    /**
     * 设置element信息
     * @param dependElements
     * @param name
     * @param loop
     * @param paramManager
     * @paarm eventManager
     * @return
     */
    virtual CStatus setElementInfo(const std::set<GElement *>& dependElements,
                                   const std::string& name,
                                   CSize loop,
                                   GParamManagerPtr paramManager,
                                   GEventManagerPtr eventManager);

    /**
     * 获取当前element内部参数
     * @tparam T
     * @param key
     * @return
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<GElementParam, T>::value, int> = 0>
    T* getEParam(const std::string& key);

    /**
     * 包含切面相关功能的函数，fat取自fatjar的意思
     * @param type
     * @return
     */
    CStatus fatProcessor(const CFunctionType& type);

    /**
     * 获取执行线程对应的信息
     * @return
     * @notice 辅助线程返回-1
     */
    CIndex getThreadNum();

    /**
     * 触发一个事件
     * @param key
     * @param type
     * @return
     * @notice 返回值仅表示是否触发成功，不表示事件是否执行成功
     */
    CStatus notify(const std::string& key, GEventType type);

    /**
     * 设置线程池信息
     * @param ptr
     * @return
     */
    GElement* setThreadPool(UThreadPoolPtr ptr);

    /**
     * graphviz dump 逻辑
     * @param oss
     * @return
    */
    virtual CVoid dump(std::ostream& oss);

    /**
     * graphviz dump 边逻辑
     * @param oss
     * @param src 表示开始的元素
     * @param dst 表示结束的元素
     * @param label
     * @return
    */
    CVoid dumpEdge(std::ostream& oss, GElement* src, GElement* dst, const std::string& label="");

    /**
     * graphviz dump 点逻辑
     * @param oss
    */
    CVoid dumpElement(std::ostream& oss);

    /**
     * 当前element是否是一个 group逻辑
     * @return
    */
    CBool isGroup();

    CGRAPH_NO_ALLOWED_COPY(GElement);

    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER

protected:
    CBool done_ { false };                           // 判定被执行结束
    CBool linkable_ { false };                       // 判定是否可以连通计算
    CSize loop_ { CGRAPH_DEFAULT_LOOP_TIMES };       // 元素执行次数
    CLevel level_ { CGRAPH_DEFAULT_ELEMENT_LEVEL };  // 用于设定init的执行顺序(值小的，优先init，可以为负数)
    std::set<GElement *> run_before_;                // 被依赖的节点
    std::set<GElement *> dependence_;                // 依赖的节点信息
    std::atomic<CSize> left_depend_ { 0 };        // 当 left_depend_ 值为0的时候，即可以执行该element信息
    GParamManagerPtr param_manager_ { nullptr };     // 整体流程的参数管理类，所有pipeline中的所有element共享
    GAspectManagerPtr aspect_manager_ { nullptr };   // 整体流程的切面管理类
    GEventManagerPtr event_manager_ { nullptr };     // 事件管理类
    UThreadPoolPtr thread_pool_ { nullptr };         // 用于执行的线程池信息
    GElementParamMap local_params_;                  // 用于记录当前element的内部参数
    GElementType element_type_;                      // 用于区分element 内部类型

    friend class GNode;
    friend class GCluster;
    friend class GRegion;
    friend class GCondition;
    friend class GElementManager;
    friend class GGroup;
    friend class GPipeline;
    friend class GElementSorter;
    friend class GStaticEngine;
    friend class GDynamicEngine;
    template<typename T> friend class GSingleton;
};

using GElementPtr = GElement *;
using GElementPtrArr = std::vector<GElementPtr>;
using GElementPtrSet = std::set<GElementPtr>;
using GElementPtrSetRef = GElementPtrSet &;

CGRAPH_NAMESPACE_END

#include "GElement.inl"

#endif //CGRAPH_GELEMENT_H
