/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPipeline.inl
@Time: 2021/4/26 9:16 下午
@Desc:
***************************/

#ifndef CGRAPH_GPIPELINE_INL
#define CGRAPH_GPIPELINE_INL

#include <algorithm>

template<typename T, std::enable_if_t<std::is_base_of_v<GElement, T>, int>>
CSTATUS GPipeline::registerGElement(GElementPtr *elementRef,
                                    const GElementPtrSet &dependElements,
                                    const std::string &name,
                                    int loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    if (element_manager_->hasElement(*elementRef)) {
        element_manager_->deleteElement(*elementRef);    // 每次注册，都默认为是新的节点
    }

    if constexpr (std::is_base_of_v<GGroup, T>) {
        /**
         * 如果是GGroup类型的信息，则：
         * 1，必须外部创建
         * 2，未被注册到其他的pipeline中
         */
        if ((*elementRef) != nullptr
            && (*elementRef)->param_manager_ != nullptr) {
            CGRAPH_ECHO("This group has already been registered to another pipeline.");
            return STATUS_ERR;
        }
    } else if constexpr (std::is_base_of_v<GNode, T>) {
        /**
         * 如果不是group信息的话，且属于element（包含node和aspect）
         * 则直接内部创建该信息
         */
        (*elementRef) = new(std::nothrow) T();
    }

    CGRAPH_ASSERT_NOT_NULL(*elementRef)
    status = (*elementRef)->setElementInfo(dependElements, name, loop, this->param_manager_);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->addElement(dynamic_cast<GElementPtr>(*elementRef));
    CGRAPH_FUNCTION_CHECK_STATUS
    element_repository_.insert(*elementRef);
    CGRAPH_FUNCTION_END
}


template<typename T, std::enable_if_t<std::is_base_of_v<GNode, T>, int>>
GNodePtr GPipeline::createGNode(const GNodeInfo &info) {
    CGRAPH_FUNCTION_BEGIN
    GNodePtr node = new(std::nothrow) T();
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(node)

    status = node->setElementInfo(info.dependence, info.name, info.loop, this->param_manager_);
    if (STATUS_OK != status) {
        CGRAPH_DELETE_PTR(node);
        return nullptr;
    }

    element_repository_.insert(node);
    return node;
}


template<typename T, std::enable_if_t<std::is_base_of_v<GGroup, T>, int>>
GElementPtr GPipeline::createGGroup(const GElementPtrArr &elements,
                                    const GElementPtrSet &dependElements,
                                    const std::string &name,
                                    int loop) {
    CGRAPH_FUNCTION_BEGIN

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

    status = group->setElementInfo(dependElements, name, loop);    // 注册group信息的时候，不能注册paramManager信息
    if (STATUS_OK != status) {
        CGRAPH_DELETE_PTR(group)
        return nullptr;
    }

    this->element_repository_.insert(group);
    return group;
}

#endif //CGRAPH_GPIPELINE_INL
