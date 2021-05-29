/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.inl
@Time: 2021/4/26 9:16 下午
@Desc:
***************************/

#ifndef CGRAPH_GRAPHIC_INL
#define CGRAPH_GRAPHIC_INL


template<typename T>
CSTATUS Graphic::registerGraphNode(GraphNodePtr* nodeRef,
                          const std::set<GraphNodePtr>& dependNodes,
                          const std::string& name,
                          int runTimes) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(node_manage_)

    if (node_manage_->hasNode(*nodeRef)) {
        node_manage_->deleteNode(*nodeRef);    // 每次注册，都默认为是新的节点
    }

    /**
     * 如果T类型是GraphNode的子类，则new T类型的对象，并且放到graph_nodes_中去
     * 如果创建成功，则添加依赖信息。
     * 如果添加依赖信息失败，则默认创建节点失败，清空节点信息
     * */
    if (std::is_base_of<GraphNode, T>::value) {
        (*nodeRef) = new(std::nothrow) T();
        CGRAPH_ASSERT_NOT_NULL(*nodeRef)
        status = addDependNodes(*nodeRef, dependNodes);
        CGRAPH_FUNCTION_CHECK_STATUS

        (*nodeRef)->setName(name);
        (*nodeRef)->setRunTimes(runTimes);
        status = node_manage_->insertNode(dynamic_cast<GraphNodePtr>(*nodeRef));
        CGRAPH_FUNCTION_CHECK_STATUS
    } else {
        status = STATUS_ERR;
    }

    CGRAPH_FUNCTION_END
}

#endif //CGRAPH_GRAPHIC_INL
