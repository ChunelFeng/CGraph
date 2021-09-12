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

#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"
#include "../GraphParam/GParamInclude.h"

class GElement : public CObject {
public:
    /**
     * 获取name信息
     */
    const std::string& getName() const;

    /**
     * 获取session信息（全局唯一）
     */
    const std::string& getSession() const;

    /**
     * 获取参数信息，如果未找到，则返回nullptr
     * @tparam T
     * @param key
     * @return
     */
    template<typename T>
    T* getGParam(const std::string& key);

    /**
     * 创建param信息，如果过了，则直接返回ok
     * @tparam T
     * @param key
     * @return
     */
    template<typename T>
    CSTATUS createGParam(const std::string& key);

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
     * 实现拷贝构造函数
     * @param element
     */
    GElement(const GElement& element);

    /**
     * 赋值构造函数
     * @param element
     * @return
     */
    GElement& operator=(const GElement& element);

    /**
     * run方法执行之前的执行函数
     * @return
     */
    virtual CSTATUS beforeRun() = 0;

    /**
     * run方法执行之后的执行函数
     * @return
     */
    virtual CSTATUS afterRun() = 0;

    /**
     * 设置name信息
     * @param name
     */
    void setName(const std::string& name);

    /**
     * 设置循环次数
     * @param loop
     */
    void setLoop(int loop = 1);

    /**
     * 判定element是否可以运行
     * 可执行的条件为：自身未被执行且依赖节点全部被执行
     * @return
     */
    [[nodiscard]] bool isRunnable() const;

    /**
     * 判定node是否可以和前面节点一起执行
     * @return
     */
    [[nodiscard]] bool isLinkable() const;

    /**
     * 线程池中的运行函数，依次执行beforeRun，run和afterRun方法，
     * 其中有任何返回值问题，则直接返回
     * @param isMock 是否真实执行run方法。默认执行的
     * @return
     */
    virtual CSTATUS process(bool isMock);

    /**
     * 设置当前参数管理类信息
     * @param manager
     * @return
     */
    virtual CSTATUS setParamManager(const GParamManagerPtr manager);

protected:
    bool done_ { false };                            // 判定被执行结束
    bool is_init_ { false };                         // 是否初始化了
    bool linkable_ { false };                        // 判定是否可以连通计算
    int loop_ { 1 };                                 // 节点执行次数
    std::string name_;                               // 节点名称
    std::string session_;                            // 节点唯一id信息
    std::set<GElement *> run_before_;                // 被依赖的节点
    std::set<GElement *> dependence_;                // 依赖的节点信息
    std::atomic<int> left_depend_{ 0 };           // 当 left_depend_ 值为0的时候，即可以执行该node信息
    GParamManagerPtr param_manager_ { nullptr };     // 整体流程的参数管理类，所有pipeline中的所有节点共享

    friend class GNode;
    friend class GCluster;
    friend class GRegion;
    friend class GCondition;
    friend class GElementManager;
    friend class GGroup;
    friend class GPipeline;
};

using GElementPtr = GElement *;
using GElementPtrArr = std::vector<GElementPtr>;
using GElementPtrSet = std::set<GElementPtr>;

#include "GElement.inl"

#endif //CGRAPH_GELEMENT_H
