/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GSingleton.h
@Time: 2021/10/30 10:24 下午
@Desc: 
***************************/

#ifndef CGRAPH_GSINGLETON_H
#define CGRAPH_GSINGLETON_H

#include "../GAdapter.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T>
class GSingleton : public GAdapter {
protected:
    CSTATUS init() override;
    CSTATUS run() override;
    CSTATUS deinit() override;

    /**
     * 适配singleton信息
     * @param dependElements
     * @param name
     * @param loop
     * @param paramManager
     * @return
     */
    CSTATUS setElementInfo(const std::set<GElement *> &dependElements,
                           const std::string &name,
                           int loop,
                           GParamManagerPtr paramManager) override;

private:
    static USingleton<T> s_singleton_;
    static std::atomic<bool> s_is_init_;

    friend class GPipeline;
};

template<typename T>
using GSingletonPtr = GSingleton<T> *;

CGRAPH_NAMESPACE_END

#include "GSingleton.inl"

#endif //CGRAPH_GSINGLETON_H
