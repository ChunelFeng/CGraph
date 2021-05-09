/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphNode.h
@Time: 2021/4/26 9:29 下午
@Desc: GraphNode类有纯虚函数run，无法直接new。继承本类，必须实现run方法
***************************/

#ifndef CGRAPH_GRAPHNODE_H
#define CGRAPH_GRAPHNODE_H

#include <atomic>
#include <set>

#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"

class GraphNode : public CObject {
public:
    explicit GraphNode();

    CSTATUS init() override;

    CSTATUS deinit() override;

    ~GraphNode() override;

    /**
     * 实现拷贝构造函数
     * @param node
     */
    GraphNode(const GraphNode& node);

    /**
     * 赋值构造函数
     * @param node
     * @return
     */
    GraphNode& operator=(const GraphNode& node);

protected:
    /**
     * run方法执行之前的执行函数
     * @return
     */
    CSTATUS beforeRun();

    /**
     * run方法执行之后的执行函数
     * @return
     */
    CSTATUS afterRun();

    /**
     * 线程池中的运行函数，依次执行beforeRun，run和afterRun方法，
     * 其中有任何返回值问题，则直接返回
     * @return
     */
    CSTATUS process();

    /**
     * 判定node是否可以运行
     * 可执行的条件为：自身未被执行且依赖节点全部被执行
     * nodiscard 表示返回值必须被判断
     * @return
     */
    [[nodiscard]]bool isRunnable() const;

    /**
     * 判定node是否可以和前面节点一起执行
     * @return
     */
    [[nodiscard]]bool isLinkable() const;

private:
    std::atomic<bool> done_ {false};          // 标记被执行结束
    std::set<GraphNode *> run_before_;        // 被依赖的节点
    std::set<GraphNode *> dependence_;        // 依赖的节点信息（做去重和计数使用）
    int left_depend_;                         // 当 left_depend_ 值为0的时候，即可以执行该node信息
    bool linkable_{ false };                  // 判定是否可以连通计算

    /* 设置友元类，使得在Graphic和GraphThreadPool中可以获取本类的信息
     * 且外部无法继承或者修改对应函数 */
    friend class Graphic;
    friend class GraphThreadPool;
    friend class GraphNodeManager;
    friend class GraphNodeCluster;
};


#endif //CGRAPH_GRAPHNODE_H
