/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CFLow.inl
@Time: 2021/4/26 9:16 下午
@Desc:
***************************/

#ifndef CGRAPH_GFLOW_INL
#define CGRAPH_GFLOW_INL


template<typename T>
CSTATUS GFlow::registerGElement(GElementPtr* elementRef,
                                const GElementPtrSet& dependElements,
                                const std::string& name,
                                int loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    if (manager_->hasElement(*elementRef)) {
        manager_->deleteElement(*elementRef);    // 每次注册，都默认为是新的节点
    }

    /**
     * 如果T类型是 GElement 的子类，则new T类型的对象，并且放到 manager_ 中去
     * 如果创建成功，则添加依赖信息。
     * 如果添加依赖信息失败，则默认创建节点失败，清空节点信息
     * */
    if (std::is_base_of<GNode, T>::value) {
        (*elementRef) = new(std::nothrow) T();
        CGRAPH_ASSERT_NOT_NULL(*elementRef)
    } else if (std::is_base_of<GCluster, T>::value) {
        CGRAPH_ASSERT_NOT_NULL(elementRef)
    } else if (std::is_base_of<GRegion, T>::value) {
        CGRAPH_ASSERT_NOT_NULL(elementRef)
    } else {
        return STATUS_ERR;
    }

    (*elementRef)->setName(name);
    (*elementRef)->setLoop(loop);
    status = addDependElements(*elementRef, dependElements);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = manager_->addElement(dynamic_cast<GElementPtr>(*elementRef));
    element_repository_.insert(*elementRef);
    CGRAPH_FUNCTION_END
}


template<typename T>
GNodePtr GFlow::createGNode(const GNodeInfo& info) {
    GNodePtr node = nullptr;
    if (std::is_base_of<GNode, T>::value) {
        node = new(std::nothrow) T();

        node->setName(info.name);
        node->setLoop(info.loop);
        node->dependence_ = info.dependence;
        node->left_depend_ = node->dependence_.size();

        for (GElementPtr ptr : node->dependence_) {
            ptr->run_before_.insert(node);
        }

        element_repository_.insert(node);
    }

    return node;
}


#endif //CGRAPH_CFLOW_INL
