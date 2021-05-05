/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphCtrl.h
@Time: 2021/4/26 9:16 下午
@Desc: 
***************************/

#ifndef CGRAPH_GRAPHIC_H
#define CGRAPH_GRAPHIC_H

#include <list>
#include <queue>

#include "../../CObject/CObject.h"
#include "../GraphNode/GraphNode.h"
#include "../GraphThread/GraphThreadPool.h"
#include "../GraphDefine.h"


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
    T* registerGraphNode(const std::set<GraphNode *>& dependNodes = std::initializer_list<GraphNode *>()) {
        if (true == is_init_) {
            return nullptr;    // 图初始化之后，则不允许创建节点
        }

        T* node = nullptr;
        /**
         * 如果T类型是GraphNode的子类，则new T类型的对象，并且放到graph_nodes_中去
         * 如果创建成功，则添加依赖信息。
         * 如果添加依赖信息失败，则默认创建节点失败，清空节点信息
         * */

        if (std::is_base_of<GraphNode, T>::value) {
            node = new(std::nothrow) T();
            if (node) {
                CSTATUS status = addDependNodes(node, dependNodes);
                if (STATUS_OK == status) {
                    graph_nodes_.emplace_back(dynamic_cast<GraphNode *>(node));
                } else {
                    CGRAPH_DELETE_PTR(node)
                }
            }
        }
        return node;
    }

    /**
     * node节点，添加依赖节点信息
     * @param node
     * @param dependNodes
     * @return
     */
    CSTATUS addDependNodes(GraphNode* node,
                           const std::set<GraphNode *>& dependNodes);

protected:
    /**
     * 确认图的最终执行状态
     * @return
     */
    CSTATUS checkFinalStatus(int runNodeSize);

private:
    std::queue<GraphNode *> queue_;          // 计算后的数据
    std::list<GraphNode *> graph_nodes_;     // 插进来的数据
    GraphThreadPool* thread_pool_;           // 线程池
    bool is_init_;    // 标记是否已经初始化完毕
};


#endif //CGRAPH_GRAPHIC_H
