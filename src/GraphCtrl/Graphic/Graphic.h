/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.h
@Time: 2021/4/26 9:16 下午
@Desc: 
***************************/

#ifndef CGRAPH_GRAPHIC_H
#define CGRAPH_GRAPHIC_H

#include <list>
#include <queue>
#include <memory>

#include "../../CObject/CObject.h"
#include "../GraphNode/GraphNode.h"
#include "../GraphNode/GraphNodeCluster.h"
#include "../GraphNode/GraphNodeLoopRegion.h"
#include "../GraphNode/GraphNodeManager.h"
#include "../GraphThread/GraphThreadPool.h"
#include "../GraphDefine.h"

/*  */
using LoopRegionArr = std::vector<GraphNodeLoopRegion>;

class Graphic : public CObject {
public:
    /**
    * 构造函数
    */
    Graphic();

    /**
     * 初始化函数
     */
    CSTATUS init() override;

    /**
     * 流程处理函数，必须实现
     * @return
     */
    CSTATUS run() override;

    /**
     * 逆初始化函数
     * @return
     */
    CSTATUS deinit() override;

    /**
     * 析构函数
     */
    ~Graphic() override;

    /**
     * 在图中注册一个节点信息
     * @tparam T
     * @return
     */
    template<typename T>
    CSTATUS registerGraphNode(GraphNodePtr* nodeRef,
            const std::set<GraphNodePtr>& dependNodes = std::initializer_list<GraphNodePtr>(),
            const std::string& name = "",
            int runTimes = 1);

    /**
     * node节点，添加依赖节点信息
     * @param node
     * @param dependNodes
     * @return
     */
    CSTATUS addDependNodes(GraphNodePtr node,
                           const std::set<GraphNodePtr>& dependNodes) const;

    /**
     * 设置循环执行区域
     * @param start
     * @param end
     * @param loop
     * @return
     */
    CSTATUS setLoopRegion(const GraphNodePtr start,
                          const GraphNodePtr end,
                          int loop);

protected:
    /**
     * 确认图的最终执行状态
     * @return
     */
    CSTATUS checkFinalStatus(int runNodeSize);

    /**
     * 解析graph信息
     * @return
     */
    CSTATUS analyse();

    /**
     * 解析dag逻辑，确定区分cluster信息
     * @return
     */
    CSTATUS analyseCluster();

    /**
     * 解析dag逻辑，区分loopRegion信息
     * @return
     */
    CSTATUS analyseLoopRegion();

private:
    ParaWorkedClusterArr para_worked_clusters_;    // 循环节点的信息
    LoopRegionArr loop_regions_;                       // 解析图中的LoopRegion信息
    std::unique_ptr<GraphNodeManager> node_manage_;       // 节点管理的内容
    std::unique_ptr<GraphThreadPool> thread_pool_;        // 线程池
    bool is_init_;                                        // 标记是否已经初始化完毕
};

#include "Graphic.inl"

#endif    // CGRAPH_GRAPHIC_H
