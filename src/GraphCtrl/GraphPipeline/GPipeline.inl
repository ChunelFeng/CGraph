/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CFLow.inl
@Time: 2021/4/26 9:16 下午
@Desc:
***************************/

#ifndef CGRAPH_GPIPELINE_INL
#define CGRAPH_GPIPELINE_INL

#include <algorithm>

template<typename T>
CSTATUS GPipeline::registerGElement(GElementPtr* elementRef,
                                    const GElementPtrSet& dependElements,
                                    const std::string& name,
                                    int loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    if (element_manager_->hasElement(*elementRef)) {
        element_manager_->deleteElement(*elementRef);    // 每次注册，都默认为是新的节点
    }

    /**
     * 如果是GNode类型，则直接创建节点
     * 如果不是GNode类型，则需要外部创建好，然后注册进来
     * */
    if (std::is_base_of<GNode, T>::value) {
        (*elementRef) = new(std::nothrow) T();
    }

    CGRAPH_ASSERT_NOT_NULL(elementRef)
    status = (*elementRef)->setParamManager(this->param_manager_);
    CGRAPH_FUNCTION_CHECK_STATUS

    (*elementRef)->setName(name);
    (*elementRef)->setLoop(loop);
    status = addDependElements(*elementRef, dependElements);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->addElement(dynamic_cast<GElementPtr>(*elementRef));
    CGRAPH_FUNCTION_CHECK_STATUS
    element_repository_.insert(*elementRef);
    CGRAPH_FUNCTION_END
}


template<typename T>
GElementPtr GPipeline::createGNode(const GNodeInfo& info) {
    GNodePtr node = nullptr;
    if (std::is_base_of<GNode, T>::value) {
        node = new(std::nothrow) T();
        CSTATUS status = addDependElements(node, info.dependence);
        if (STATUS_OK != status) {
            return nullptr;
        }
        node->setName(info.name);
        node->setLoop(info.loop);
        node->setParamManager(this->param_manager_);    // 设置参数信息类
        element_repository_.insert(node);
    }

    return node;
}


template<typename T>
GElementPtr GPipeline::createGGroup(const GElementPtrArr& elements,
                                    const GElementPtrSet& dependElements,
                                    const std::string& name,
                                    int loop) {
    // 如果不是所有的都非空，则创建失败
    if (std::any_of(elements.begin(), elements.end(),
                     [](GElementPtr element) {
                        return (nullptr == element);
                     })) {
        return nullptr;
    }

    if (std::any_of(dependElements.begin(), dependElements.end(),
                     [](GElementPtr element) {
                         return (nullptr == element);
                     })) {
        return nullptr;
    }

    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(this->thread_pool_)    // 所有的pipeline必须有线程池

    GGroupPtr group = new(std::nothrow) T();
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(group)
    for (GElementPtr element : elements) {
        group->addElement(element);
    }

    if (std::is_same<GRegion, T>::value) {
        // 如果是GRegion类型，需要设置线程池信息。因为GRegion内部可能需要并行
        ((GRegion *)group)->setThreadPool(this->thread_pool_);
    }

    CSTATUS status = addDependElements(group, dependElements);
    if (STATUS_OK != status) {
        CGRAPH_DELETE_PTR(group)
        return nullptr;
    }
    group->setName(name);
    group->setLoop(loop);
    this->element_repository_.insert(group);
    return group;
}


#endif //CGRAPH_CFLOW_INL
