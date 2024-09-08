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

#include "GPipeline.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T,
        c_enable_if_t<std::is_base_of<GElement, T>::value, int>>
CStatus GPipeline::registerGElement(GElementPPtr elementRef,
                                    const GElementPtrSet &depends,
                                    const std::string &name,
                                    CSize loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    if (std::is_base_of<GGroup, T>::value) {
        /**
         * 如果是GGroup类型的信息，则：
         * 1，必须外部创建
         * 2，未被注册到其他的pipeline中
         */
        if ((*elementRef) != nullptr
            && ((*elementRef)->isRegistered())) {
            CGRAPH_RETURN_ERROR_STATUS("this group register duplicate")
        }
    } else if (std::is_base_of<GNode, T>::value || std::is_base_of<GAdapter, T>::value) {
        /**
         * 如果不是group信息的话，且属于element（包含node和adapter）
         * 则直接内部创建该信息
         */
        (*elementRef) = new(std::nothrow) T();
    } else {
        CGRAPH_RETURN_ERROR_STATUS("resister error type")
    }

    status = innerRegister(*elementRef, depends, name, loop);
    CGRAPH_FUNCTION_END
}


template<typename TNode,
        c_enable_if_t<std::is_base_of<GNode, TNode>::value, int>>
TNode* GPipeline::registerGNode(const GElementPtrSet &depends,
                                const std::string &name, CSize loop) {
    GElementPtr node = nullptr;
    CGRAPH_THROW_EXCEPTION_BY_STATUS(registerGElement<TNode>(&node, depends, name, loop))
    return (TNode *)node;
}


template<typename TNode, typename ...Args,
        c_enable_if_t<std::is_base_of<GTemplateNode<Args ...>, TNode>::value, int>>
TNode* GPipeline::registerGNode(const GElementPtrSet &depends,
                                Args... args) {
    GTemplateNodePtr<Args ...> node = nullptr;
    CGRAPH_THROW_EXCEPTION_BY_STATUS(registerGElement<TNode>(&node, depends, args...))
    return (TNode *)node;
}


template<typename TNode, typename ...Args,
        c_enable_if_t<std::is_base_of<GTemplateNode<Args ...>, TNode>::value, int>>
CStatus GPipeline::registerGElement(GTemplateNodePtr<Args ...> *elementRef,
                                    const GElementPtrSet &depends,
                                    Args... args) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    // 构建模板node信息
    (*elementRef) = new(std::nothrow) TNode(std::forward<Args &&>(args)...);
    CGRAPH_ASSERT_NOT_NULL(*elementRef)

    status = innerRegister(*elementRef, depends, CGRAPH_EMPTY, CGRAPH_DEFAULT_LOOP_TIMES);
    CGRAPH_FUNCTION_END
}


template<typename GFunction>
CStatus GPipeline::registerGElement(GFunctionPPtr functionRef,
                                    const GElementPtrSet &depends,
                                    const std::string &name,
                                    CSize loop) {
    // 通过模板特化的方式，简化 GFunction 的注册方式
    return this->registerGElement<GFunction>((GElementPtr *)(functionRef), depends, name, loop);
}


template<typename GFence>
CStatus GPipeline::registerGElement(GFencePPtr fenceRef,
                                    const GElementPtrSet &depends,
                                    const std::string &name,
                                    CSize loop) {
    return this->registerGElement<GFence>((GElementPtr *)(fenceRef), depends, name, loop);
}


template<typename GCoordinator, CInt SIZE>
CStatus GPipeline::registerGElement(GCoordinatorPPtr<SIZE> coordinatorRef,
                                    const GElementPtrSet &depends,
                                    const std::string &name,
                                    CSize loop) {
    return this->registerGElement<GCoordinator, SIZE>((GElementPtr *)(coordinatorRef), depends, name, loop);
}


template<typename TNode, typename ...Args,
        c_enable_if_t<std::is_base_of<GNode, TNode>::value, int>>
TNode* GPipeline::createGNode(const GNodeInfo &info, Args&&... args) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)

    auto* node = new(std::nothrow) TNode(std::forward<Args &&>(args)...);
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(node)
    status = node->addElementInfo(info.dependence_, info.name_, info.loop_);
    CGRAPH_THROW_EXCEPTION_BY_STATUS(status)

    repository_.insert(node);
    return node;
}


template<typename TNode, typename ...Args,
        c_enable_if_t<std::is_base_of<GNode, TNode>::value, int>>
TNode* GPipeline::createGNode(const GElementPtrSet& dependence, const std::string& name,
                                CSize loop, Args&&... args) {
    const GNodeInfo& info = GNodeInfo(dependence, name, loop);
    return createGNode<TNode>(info, std::forward<Args &&>(args)...);
}


template<typename TGroup,
        c_enable_if_t<std::is_base_of<GGroup, TGroup>::value, int>>
TGroup* GPipeline::createGGroup(const GElementPtrArr &elements,
                           const GElementPtrSet &depends,
                           const std::string &name,
                           CSize loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)

    // 如果不是所有的都非空，则创建失败
    CGRAPH_THROW_EXCEPTION_BY_CONDITION(std::any_of(elements.begin(), elements.end(),
                                                    [](GElementPtr element) { return (nullptr == element); }),
                                        "createGGroup elements have nullptr.")
    CGRAPH_THROW_EXCEPTION_BY_CONDITION(std::any_of(depends.begin(), depends.end(),
                                                    [](GElementPtr element) { return (nullptr == element); }),
                                        "createGGroup depends have nullptr.")

    auto* group = CGRAPH_SAFE_MALLOC_COBJECT(TGroup)
    for (GElementPtr element : elements) {
        status += group->addElement(element);
        element->belong_ = group;    // 从属于这个group的信息
    }
    CGRAPH_THROW_EXCEPTION_BY_STATUS(status)

    // 加入group的时候，是不设定manager信息的
    status = group->addElementInfo(depends, name, loop);
    CGRAPH_THROW_EXCEPTION_BY_STATUS(status)

    this->repository_.insert(group);
    return group;
}


template<typename TAspect, typename TParam,
        c_enable_if_t<std::is_base_of<GAspect, TAspect>::value, int>,
        c_enable_if_t<std::is_base_of<GAspectParam, TParam>::value, int>>
GPipelinePtr GPipeline::addGAspect(const GElementPtrSet& elements, TParam* param) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)

    // 如果传入的是空的话，则默认将所有的element添加aspect信息
    auto curElements = elements.empty() ? repository_.elements_ : elements;
    for (GElementPtr element : curElements) {
        if (repository_.find(element)) {
            element->addGAspect<TAspect, TParam>(param);
        }
    }

    return this;
}


template<typename TDaemon, typename TParam,
        c_enable_if_t<std::is_base_of<GDaemon, TDaemon>::value, int>,
        c_enable_if_t<std::is_base_of<GDaemonParam, TParam>::value, int>>
GPipelinePtr GPipeline::addGDaemon(CMSec ms, TParam* param) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(param_manager_, daemon_manager_)

    GDaemonPtr daemon = CGRAPH_SAFE_MALLOC_COBJECT(TDaemon)
    daemon->setDParam<TParam>(param)->setInterval(ms);
    daemon->setGParamManager(this->param_manager_);
    daemon->setGEventManager(this->event_manager_);
    status = daemon_manager_->add(daemon);
    CGRAPH_THROW_EXCEPTION_BY_STATUS(status)

    return this;
}


template<typename TDaemon, typename ...Args,
        c_enable_if_t<std::is_base_of<GTemplateDaemon<Args...>, TDaemon>::value, int>>
GPipelinePtr GPipeline::addGDaemon(CMSec ms, Args&&... args) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(param_manager_, daemon_manager_)

    auto daemon = UAllocator::safeMallocTemplateCObject<TDaemon>(std::forward<Args &&>(args)...);
    daemon->setInterval(ms);
    daemon->setGParamManager(this->param_manager_);
    daemon->setGEventManager(this->event_manager_);
    status = daemon_manager_->add(daemon);
    CGRAPH_THROW_EXCEPTION_BY_STATUS(status)

    return this;
}


template<typename TEvent, typename TParam,
        c_enable_if_t<std::is_base_of<GEvent, TEvent>::value, int>,
        c_enable_if_t<std::is_base_of<GEventParam, TParam>::value, int>>
GPipelinePtr GPipeline::addGEvent(const std::string& key, TParam* param) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(param_manager_, event_manager_)

    event_manager_->param_manager_ = this->param_manager_;
    status = event_manager_->createWithParam<TEvent, TParam>(key, param);
    CGRAPH_THROW_EXCEPTION_BY_STATUS(status)

    return this;
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPIPELINE_INL
