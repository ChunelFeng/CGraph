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
     * 如果T类型是 GElement 的子类，则new T类型的对象，并且放到 element_manager_ 中去
     * 如果创建成功，则添加依赖信息。
     * 如果添加依赖信息失败，则默认创建节点失败，清空节点信息
     * */
    if (std::is_base_of<GNode, T>::value) {
        (*elementRef) = new(std::nothrow) T();
        CGRAPH_ASSERT_NOT_NULL(*elementRef)
        status = ((GNodePtr)(*elementRef))->setParamManager(this->param_manager_);
        CGRAPH_FUNCTION_CHECK_STATUS
    } else if (std::is_same<GCluster, T>::value) {
        CGRAPH_ASSERT_NOT_NULL(elementRef)
    } else if (std::is_same<GRegion, T>::value) {
        CGRAPH_ASSERT_NOT_NULL(elementRef)
    } else {
        return STATUS_ERR;
    }

    (*elementRef)->setName(name);
    (*elementRef)->setLoop(loop);
    status = addDependElements(*elementRef, dependElements);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->addElement(dynamic_cast<GElementPtr>(*elementRef));
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
GElementPtr GPipeline::createGBlock(const GElementPtrArr& elements,
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

    GBlockPtr block = new(std::nothrow) T();
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(block)
    for (GElementPtr element : elements) {
        block->addElement(element);
    }

    if (std::is_same<GRegion, T>::value) {
        // 如果是GRegion类型，需要设置线程池信息。因为GRegion内部可能需要并行
        ((GRegion *)block)->setThreadPool(this->thread_pool_);
    }

    CSTATUS status = addDependElements(block, dependElements);
    if (STATUS_OK != status) {
        CGRAPH_DELETE_PTR(block)
        return nullptr;
    }
    block->setName(name);
    block->setLoop(loop);
    this->element_repository_.insert(block);
    return block;
}


#endif //CGRAPH_CFLOW_INL
