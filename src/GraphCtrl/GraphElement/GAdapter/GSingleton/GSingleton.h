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
    explicit GSingleton() {
        this->element_type_ = GElementType::SINGLETON;
        session_ = URandom<>::generateSession(CGRAPH_STR_SINGLETON);
    }

    CStatus init() override;
    CStatus run() override;
    CStatus destroy() override;

    /**
     * 适配singleton信息
     * @param dependElements
     * @param name
     * @param loop
     * @param paramManager
     * @param eventManager
     * @return
     */
    CStatus setElementInfo(const std::set<GElementPtr> &dependElements,
                           const std::string &name,
                           CSize loop,
                           GParamManagerPtr paramManager,
                           GEventManagerPtr eventManager) override;

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
