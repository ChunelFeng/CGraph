/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GSingleton.cpp
@Time: 2021/10/30 10:24 下午
@Desc:
***************************/

#ifndef CGRAPH_GSINGLETON_INL
#define CGRAPH_GSINGLETON_INL

#include "GSingleton.h"

CGRAPH_NAMESPACE_BEGIN

template <typename T>
USingleton<T> GSingleton<T>::s_singleton_;


template <typename T>
std::atomic<CBool> GSingleton<T>::s_is_init_(false);


template <typename T>
CStatus GSingleton<T>::init() {
    CGRAPH_FUNCTION_BEGIN
    /* 确保仅 GSingletonNode 类型内容，init一次 */
    if (s_is_init_) {
        CGRAPH_FUNCTION_END
    }

    /* 因为采取的是饥汉模式，不需要判断ptr是否为空了 */
    auto element = dynamic_cast<GElementPtr>(s_singleton_.get());
    status = element->init();
    if (status.isOK()) {
        s_is_init_ = true;
    }

    CGRAPH_FUNCTION_END
}


template <typename T>
CStatus GSingleton<T>::run() {
    CGRAPH_FUNCTION_BEGIN

    auto element = dynamic_cast<T *>(s_singleton_.get());
    status = element->run();
    CGRAPH_FUNCTION_END
}


template <typename T>
CStatus GSingleton<T>::destroy() {
    CGRAPH_FUNCTION_BEGIN
    if (!s_is_init_) {
        CGRAPH_FUNCTION_END
    }

    auto element = dynamic_cast<GElementPtr>(s_singleton_.get());
    status = element->destroy();
    if (status.isOK()) {
        s_is_init_ = false;
    }

    CGRAPH_FUNCTION_END
}


template <typename T>
CStatus GSingleton<T>::setElementInfo(const std::set<GElementPtr> &dependElements,
                                      const std::string &name,
                                      CSize loop,
                                      GParamManagerPtr paramManager,
                                      GEventManagerPtr eventManager) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(paramManager, eventManager)

    // 这里，内部和外部均需要设定name信息
    this->setName(name)->setLoop(loop);
    status = this->addDependGElements(dependElements);
    CGRAPH_FUNCTION_CHECK_STATUS

    // 获取单例信息，然后将信息node中信息
    auto element = dynamic_cast<GElementPtr>(s_singleton_.get());
    if (element->param_manager_) {
        // 设置一次即可，不支持多次设置
        CGRAPH_FUNCTION_END
    }

    /**
     * 内部不需要设置loop信息了，因为是根据adapter的loop次数循环的。
     * 依赖关系也注册在adapter上
     */
    element->param_manager_ = paramManager;
    element->event_manager_ = eventManager;
    element->name_ = name;
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSINGLETON_INL
