/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GSingleton.h
@Time: 2021/10/30 10:24 下午
@Desc: 本功能，仅在C++17或以上版本生效
***************************/

#ifndef CGRAPH_GSINGLETON_H
#define CGRAPH_GSINGLETON_H

#include "../GAdapter.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T>
class GSingleton : public GAdapter {
private:
    explicit GSingleton();

    CStatus init() final;
    CStatus run() final;
    CStatus destroy() final;

    CStatus addElementInfo(const std::set<GElementPtr> &depends,
                           const std::string &name, CSize loop) final;

    CStatus addManagers(GParamManagerPtr paramManager,
                        GEventManagerPtr eventManager) final;

    CBool isHold() final;

    CBool isMatch() final;

    CBool isRegistered() const final;

    const std::string& getName() const final;

private:
    static USingleton<T> s_singleton_;                    // 单例
    static std::atomic<CBool> s_is_init_;                 // 标志是否被初始化过

    friend class GPipeline;
};

template<typename T>
using GSingletonPtr = GSingleton<T> *;

CGRAPH_NAMESPACE_END

#include "GSingleton.inl"

#endif //CGRAPH_GSINGLETON_H
