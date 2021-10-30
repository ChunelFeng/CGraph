/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GSingletonNode.h
@Time: 2021/10/30 4:08 下午
@Desc: 
***************************/

#ifndef CGRAPH_GSINGLETONNODE_H
#define CGRAPH_GSINGLETONNODE_H

#include "GNode.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * 单例节点类型，仅支持 init/deinit 一次，并且内部生成一个对象
 * 可以多次执行
 */
template<typename T>
class GSingletonNode : public GNode {
protected:
    explicit GSingletonNode() = default;
    ~GSingletonNode() override = default;

    CSTATUS init() override {
        CGRAPH_FUNCTION_BEGIN
        /* 确保仅 GSingletonNode 类型内容，init一次 */
        if (s_is_init_) {
            CGRAPH_FUNCTION_END
        }

        auto ptr = dynamic_cast<GNodePtr>(s_singleton_.get());
        CGRAPH_ASSERT_NOT_NULL(ptr)

        status = ptr->init();
        if (status == STATUS_OK) {
            s_is_init_ = true;
        }

        CGRAPH_FUNCTION_END
    }

    CSTATUS deinit() override {
        CGRAPH_FUNCTION_BEGIN
        if (!s_is_init_) {
            CGRAPH_FUNCTION_END
        }

        auto ptr = dynamic_cast<GNodePtr>(s_singleton_.get());
        CGRAPH_ASSERT_NOT_NULL(ptr)

        status = ptr->deinit();
        if (status == STATUS_OK) {
            s_is_init_ = false;
        }

        CGRAPH_FUNCTION_END
    }

    CSTATUS run() override {
        CGRAPH_FUNCTION_BEGIN

        auto ptr = dynamic_cast<GNodePtr>(s_singleton_.get());
        CGRAPH_ASSERT_NOT_NULL(ptr)

        status = ptr->run();
        CGRAPH_FUNCTION_END
    }

private:
    static USingleton<T> s_singleton_;
    static std::atomic<bool> s_is_init_;

    friend class GPipeline;
};

template <typename T>
USingleton<T> GSingletonNode<T>::s_singleton_;

template <typename T>
std::atomic<bool> GSingletonNode<T>::s_is_init_ = false;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSINGLETONNODE_H
