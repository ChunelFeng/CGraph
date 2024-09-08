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
GSingleton<T>::GSingleton() {
    this->element_type_ = GElementType::SINGLETON;
    session_ = URandom<>::generateSession(CGRAPH_STR_SINGLETON);
}


template <typename T>
CStatus GSingleton<T>::init() {
    CGRAPH_FUNCTION_BEGIN
    /* 确保仅 GSingletonNode 类型内容，init一次 */
    if (s_is_init_) {
        CGRAPH_FUNCTION_END
    }

    /* 因为采取的是饥汉模式，不需要判断ptr是否为空了 */
    auto element = dynamic_cast<T *>(s_singleton_.get());
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

    auto element = dynamic_cast<T *>(s_singleton_.get());
    status = element->destroy();
    if (status.isOK()) {
        s_is_init_ = false;
    }

    CGRAPH_FUNCTION_END
}


template <typename T>
CStatus GSingleton<T>::addElementInfo(const std::set<GElementPtr> &depends,
                                      const std::string &name, CSize loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    status = this->addDependGElements(depends);
    CGRAPH_FUNCTION_CHECK_STATUS

    // 这里，内部和外部均需要设定name信息
    this->setName(name);
    this->setLoop(loop);
    // 获取单例信息，然后将信息node中信息
    auto element = dynamic_cast<T *>(s_singleton_.get());
    element->name_ = name;
    CGRAPH_FUNCTION_END
}


template <typename T>
CStatus GSingleton<T>::addManagers(GParamManagerPtr paramManager,
                                   GEventManagerPtr eventManager) {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(paramManager, eventManager)
    auto element = dynamic_cast<T *>(s_singleton_.get());
    status = element->addManagers(paramManager, eventManager);

    CGRAPH_FUNCTION_END
}


template <typename T>
CBool GSingleton<T>::isHold(){
    auto element = dynamic_cast<T *>(s_singleton_.get());
    return element->isHold();
}


template <typename T>
CBool GSingleton<T>::isMatch() {
    auto element = dynamic_cast<T *>(s_singleton_.get());
    return element->isMatch();
}


template <typename T>
CBool GSingleton<T>::isRegistered() const {
    auto element = dynamic_cast<T *>(s_singleton_.get());
    return element->isRegistered();
}


template <typename T>
const std::string& GSingleton<T>::getName() const {
    auto element = dynamic_cast<T *>(s_singleton_.get());
    return element->getName();
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSINGLETON_INL
