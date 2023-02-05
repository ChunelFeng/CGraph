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

CGRAPH_NAMESPACE_BEGIN

template<typename T, CLevel level, c_enable_if_t<std::is_base_of<GElement, T>::value, int>>
CStatus GPipeline::registerGElement(GElementPtr *elementRef,
                                    const GElementPtrSet &dependElements,
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
            && (*elementRef)->param_manager_ != nullptr) {
            CGRAPH_RETURN_ERROR_STATUS("group register duplicate")
        }
    } else if (std::is_base_of<GNode, T>::value || std::is_base_of<GAdapter, T>::value) {
        /**
         * 如果不是group信息的话，且属于element（包含node和adapter）
         * 则直接内部创建该信息
         */
        (*elementRef) = new(std::nothrow) T();
    }

    CGRAPH_ASSERT_NOT_NULL(*elementRef)
    status = (*elementRef)->setElementInfo(dependElements, name, loop, level,
                                           this->param_manager_,
                                           this->event_manager_);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->add(dynamic_cast<GElementPtr>(*elementRef));
    CGRAPH_FUNCTION_CHECK_STATUS
    element_repository_.insert(*elementRef);
    CGRAPH_FUNCTION_END
}


template<typename GFunction, CLevel level>
CStatus GPipeline::registerGElement(GFunctionPtr *functionRef,
                                    const GElementPtrSet &dependElements,
                                    const std::string &name,
                                    CSize loop) {
    // 通过模板特化的方式，简化 GFunction 的注册方式
    return this->registerGElement<GFunction, level>((GElementPtr *)(functionRef), dependElements, name, loop);
}


template<typename TNode, typename ...Args,
        c_enable_if_t<std::is_base_of<GTemplateNode<Args ...>, TNode>::value, int>>
CStatus GPipeline::registerGElement(GTemplateNodePtr<Args ...> *elementRef,
                                    const GElementPtrSet &dependElements,
                                    Args... args) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    // 构建模板node信息
    (*elementRef) = new(std::nothrow) TNode(std::forward<Args &&>(args)...);
    CGRAPH_ASSERT_NOT_NULL(*elementRef)
    // 以下 name，loop，level 信息，可以由外部设置
    status = (*elementRef)->setElementInfo(dependElements, CGRAPH_EMPTY, CGRAPH_DEFAULT_LOOP_TIMES,
                                           CGRAPH_DEFAULT_ELEMENT_LEVEL,
                                           this->param_manager_, this->event_manager_);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->add(dynamic_cast<GElementPtr>(*elementRef));
    CGRAPH_FUNCTION_CHECK_STATUS
    element_repository_.insert(*elementRef);
    CGRAPH_FUNCTION_END
}


template<typename T, CLevel level,
        c_enable_if_t<std::is_base_of<GNode, T>::value, int>>
GNodePtr GPipeline::createGNode(const GNodeInfo &info) {
    CGRAPH_FUNCTION_BEGIN
    GNodePtr node = CGRAPH_SAFE_MALLOC_COBJECT(T)
    status = node->setElementInfo(info.dependence_, info.name_, info.loop_, level,
                                  this->param_manager_, this->event_manager_);
    if (!status.isOK()) {
        CGRAPH_DELETE_PTR(node);
        return nullptr;
    }

    element_repository_.insert(node);
    return node;
}


template<typename T, CLevel level,
        c_enable_if_t<std::is_base_of<GGroup, T>::value, int>>
GGroupPtr GPipeline::createGGroup(const GElementPtrArr &elements,
                                  const GElementPtrSet &dependElements,
                                  const std::string &name,
                                  CSize loop) {
    CGRAPH_FUNCTION_BEGIN

    // 如果不是所有的都非空，则创建失败
    if (std::any_of(elements.begin(), elements.end(),
                    [](GElementPtr element) { return (nullptr == element); })) {
        return nullptr;
    }

    if (std::any_of(dependElements.begin(), dependElements.end(),
                    [](GElementPtr element) { return (nullptr == element); })) {
        return nullptr;
    }

    GGroupPtr group = CGRAPH_SAFE_MALLOC_COBJECT(T)
    for (GElementPtr element : elements) {
        group->addElement(element);
    }

    status = group->setElementInfo(dependElements, name, loop, level,
                                   nullptr, nullptr);    // 注册group信息的时候，不能注册paramManager信息
    if (unlikely(!status.isOK())) {
        CGRAPH_DELETE_PTR(group)
        return nullptr;
    }

    this->element_repository_.insert(group);
    return group;
}


template<typename TAspect, typename TParam,
        c_enable_if_t<std::is_base_of<GAspect, TAspect>::value, int>,
        c_enable_if_t<std::is_base_of<GAspectParam, TParam>::value, int>>
GPipelinePtr GPipeline::addGAspect(const GElementPtrSet& elements, TParam* param) {
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)

    const GElementPtrSet& curElements = elements.empty() ? element_repository_ : elements;
    for (GElementPtr element : curElements) {
        // 如果传入的为空，或者不是当前pipeline中的element，则不处理
        if (nullptr == element || (element_repository_.find(element) == element_repository_.end())) {
            continue;
        }

        element->addGAspect<TAspect, TParam>(param);
    }

    return this;
}


template<typename TDaemon, typename TParam,
        c_enable_if_t<std::is_base_of<GDaemon, TDaemon>::value, int>,
        c_enable_if_t<std::is_base_of<GDaemonParam, TParam>::value, int>>
GPipeline* GPipeline::addGDaemon(CMSec ms, TParam* param) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(param_manager_)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(daemon_manager_)

    GDaemonPtr daemon = CGRAPH_SAFE_MALLOC_COBJECT(TDaemon)
    daemon->setDParam<TParam>(param)
            ->setInterval(ms);
    daemon->setGParamManager(this->param_manager_);
    status = daemon_manager_->add(daemon);

    CGRAPH_CHECK_STATUS_RETURN_THIS_OR_NULL
}


template<typename TDaemon, typename ...Args,
        c_enable_if_t<std::is_base_of<GTemplateDaemon<Args...>, TDaemon>::value, int>>
GPipeline* GPipeline::addGDaemon(CMSec ms, Args... args) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(param_manager_)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(daemon_manager_)
    auto daemon = UAllocator::safeMallocTemplateCObject<TDaemon>(std::forward<Args>(args)...);
    daemon->setInterval(ms);
    daemon->setGParamManager(this->param_manager_);
    status = daemon_manager_->add(daemon);

    CGRAPH_CHECK_STATUS_RETURN_THIS_OR_NULL
}


template<typename TEvent, typename TParam,
        c_enable_if_t<std::is_base_of<GEvent, TEvent>::value, int>,
        c_enable_if_t<std::is_base_of<GEventParam, TParam>::value, int>>
GPipeline* GPipeline::addGEvent(const std::string& key, TParam* param) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(event_manager_)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(param_manager_)

    event_manager_->param_manager_ = this->param_manager_;
    status = event_manager_->createWithParam<TEvent, TParam>(key, param);
    CGRAPH_CHECK_STATUS_RETURN_THIS_OR_NULL
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPIPELINE_INL
