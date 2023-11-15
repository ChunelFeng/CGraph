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
#include "GElementRelation.h"

CGRAPH_NAMESPACE_BEGIN

enum class GMultiConditionType;
struct GPerfInfo;

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

    /**
     * 设置visible信息。当 visible = false 的时候，算子实际不执行
     * @param visible
     * @return
     */
    GElement* setVisible(CBool visible);

    /**
     * 设定绑定的线程id
     * @param index，需要绑定的 thread id 信息
     * @return
     * @notice 本接口仅保证绑定线程优先调度，但不保证最终一定在绑定线程上执行。若不了解调度机制，不建议使用本接口，否则可能导致运行时阻塞。
     */
    GElement* setBindingIndex(CIndex index);

    /**
     * 设定当前算子的超时时间
     * @param timeout 超时时间
     * @param strategy 当超时的时候，处理的策略
     * @return
     */
    GElement* setTimeout(CMSec timeout,
                         GElementTimeoutStrategy strategy = GElementTimeoutStrategy::AS_ERROR);

    /**
     * 当前element是否是一个 group逻辑
     * @return
     */
    CBool isGroup() const;

    /**
     * 实现连续注册的语法糖，形如：
     *  (*a)-->b&c;
     *  (*b)-->d;
     *  (*c)-->d;
     *  (*b)*2;
     * @return
     */
    GElement& operator--(int) noexcept;
    GElement& operator>(GElement* element);
    GElement& operator&(GElement* element);
    GElement& operator*(CSize loop) noexcept;

protected:
    /**
     * 构造函数
     */
    explicit GElement() = default;

    /**
     * 析构函数
     */
    ~GElement() override;

    /**
     * init()后，第一次执行run之前，会执行的函数
     * @return
     * @notice 主要为了弥补init()方法，不是并发执行的缺陷。也属于单次执行的函数
     */
    virtual CStatus prepareRun();

    /**
     * 是否持续进行
     * 默认为false，表示执行且仅执行一次
     * @return
     */
    virtual CBool isHold();

    /**
     * 用于在MultiCondition中被判定，是否可以执行。
     * @return
     * @notice 默认返回false，不执行
     */
    virtual CBool isMatch();

    /**
     * 判断当前是否超时
     * @return
     */
    CBool isTimeout() const;

    /**
     * 判断当前是否可以在运行时更新图结构逻辑
     * @return
     */
    CBool isMutable() const;

    /**
     * 崩溃流程处理
     * @param ex
     * @return
     * @notice 可以自行覆写crashed方法，但不推荐。如果需要复写的话，返回值需要填写 STATUS_CRASH，否则可能出现执行异常
     */
    virtual CStatus crashed(const CException& ex);

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
     * 获取执行线程对应的信息
     * @return
     * @notice 辅助线程返回-1
     */
    CIndex getThreadIndex();

    /**
     * 获取绑定线程id信息
     * @return
     * @notice 不同的group类型，获取 binding index 的方式不同
     */
    virtual CIndex getBindingIndex();

    /**
     * 获取当前节点的相关关系信息，包含前驱、后继、从属关系
     * @param relation
     * @return
     */
    CStatus buildRelation(GElementRelation& relation);

    CGRAPH_NO_ALLOWED_COPY(GElement);

private:
    /**
     * run方法执行之前的执行函数（请勿覆写）
     * @return
     */
    virtual CVoid beforeRun();

    /**
     * run方法执行之后的执行函数（请勿覆写）
     * @return
     */
    virtual CVoid afterRun();

    /**
     * 判定element是否可以运行
     * 可执行的条件为：自身未被执行且依赖节点全部被执行
     * @return
     */
    CBool isRunnable() const;

    /**
     * 判定node是否可以和前面节点一起执行
     * @return
     */
    CBool isLinkable() const;

    /**
     * 判定当前的内容，是否需要异步执行
     * @return
     */
    CBool isAsync() const;

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
     * 包含切面相关功能的函数，fat取自fatjar的意思
     * @param type
     * @return
     */
    CStatus fatProcessor(const CFunctionType& type);

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
    CVoid dumpEdge(std::ostream& oss,
                   GElement* src,
                   GElement* dst,
                   const std::string& label = CGRAPH_EMPTY);

    /**
     * graphviz dump 点逻辑
     * @param oss
    */
    virtual CVoid dumpElement(std::ostream& oss);

    /**
     * dump 当前element的header。主要就是为了代码
     * @param oss
     * @return
     */
    CVoid dumpElementHeader(std::ostream& oss);

    /**
     * graphviz dump perf逻辑
     * @param oss
     * @return
     */
    CVoid dumpPerfInfo(std::ostream& oss);

    /**
     * 判断是否进入 yield状态。如果是的话，则等待恢复。未进入yield状态，则继续运行
     * @return
     */
    inline CVoid checkYield();

    /**
     * 判断当前元素，是否可以线性执行。默认返回true
     * @return
     */
    virtual CBool isSerializable();

    /**
     * 弹出一个最后一个切面
     * @return
     */
    CStatus popLastAspect();

    /**
     * 异步执行
     * @return
     */
    CStatus asyncRun();

    /**
     * 异步获取结果信息
     * @return
     */
    CStatus getAsyncResult();

    /**
     * 判断当前节点，是否符合执行逻辑。主要用于init的早期阶段
     * @return
     */
    virtual CStatus checkSuitable();

private:
    /** 状态相关信息 */
    CBool done_ { false };                                                    // 判定被执行结束
    CBool linkable_ { false };                                                // 判定是否可以连通计算
    CBool visible_ { true };                                                  // 判定可见的，如果被删除的话，则认为是不可见的
    CBool is_init_ { false };                                                 // 判断是否init
    GElementType element_type_ { GElementType::ELEMENT };                     // 用于区分element 内部类型
    std::atomic<GElementState> cur_state_ { GElementState::CREATE };       // 当前执行状态

    /** 配置相关信息 */
    CSize loop_ { CGRAPH_DEFAULT_LOOP_TIMES };                                // 元素执行次数
    CLevel level_ { CGRAPH_DEFAULT_ELEMENT_LEVEL };                           // 用于设定init的执行顺序(值小的，优先init，可以为负数)
    CIndex binding_index_ { CGRAPH_DEFAULT_BINDING_INDEX };                   // 用于设定绑定线程id
    CMSec timeout_ { CGRAPH_DEFAULT_ELEMENT_TIMEOUT };                        // 超时时间信息（0表示不计算超时）
    GElementTimeoutStrategy timeout_strategy_ { GElementTimeoutStrategy::AS_ERROR };    // 判定超时的情况下，是否返回错误

    /** 执行期间相关信息 */
    GElementParamMap local_params_;                                           // 用于记录当前element的内部参数
    GAspectManagerPtr aspect_manager_ { nullptr };                            // 整体流程的切面管理类
    UThreadPoolPtr thread_pool_ { nullptr };                                  // 用于执行的线程池信息
    GPerfInfo* perf_info_ = nullptr;                                          // 用于perf的信息
    CULong trigger_times_ { 0 };                                              // 被触发的次数信息（loop执行n次，算触发1次）

    /** 图相关信息 */
    std::atomic<CSize> left_depend_ { 0 };                                 // 当 left_depend_ 值为0的时候，即可以执行该element信息
    std::set<GElement *> run_before_;                                         // 被依赖的节点（后继）
    std::set<GElement *> dependence_;                                         // 依赖的节点信息（前驱）
    GElement* belong_ { nullptr };                                            // 从属的element 信息，如为nullptr，则表示从属于 pipeline

    /** 异步执行相关信息 */
    std::future<CStatus> async_result_;                                       // 用于记录当前节点的异步执行情况
    std::mutex yield_mutex_;                                                  // 控制停止执行的锁
    std::condition_variable yield_cv_;                                        // 控制停止执行的条件变量

    friend class GNode;
    friend class GGroup;
    friend class GCluster;
    friend class GRegion;
    friend class GCondition;
    friend class GMutable;
    template<CInt> friend class GSome;
    template<GMultiConditionType> friend class GMultiCondition;
    friend class GPipeline;
    friend class GElementManager;
    friend class GElementSorter;
    friend class GAdapter;
    friend class GFunction;
    friend class GFence;
    template<CInt> friend class GCoordinator;
    template<typename T> friend class GSingleton;
    friend class GEngine;
    friend class GStaticEngine;
    friend class GDynamicEngine;
    friend class GMaxParaOptimizer;
    friend class GElementRepository;
    friend class GPerf;

    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER_WITH_MEMBER
    CGRAPH_DECLARE_GEVENT_MANAGER_WRAPPER_WITH_MEMBER
};

using GElementRef = GElement &;
using GElementPtr = GElement *;
using GElementPPtr = GElementPtr *;
using GElementPtrArr = std::vector<GElementPtr>;
using GElementPtrSet = std::set<GElementPtr>;

CGRAPH_NAMESPACE_END

#include "GElement.inl"

#endif //CGRAPH_GELEMENT_H
