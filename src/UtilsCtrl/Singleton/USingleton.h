/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: SingletonProc.h
@Time: 2021/6/11 7:06 下午
@Desc: 
***************************/

#ifndef CGRAPH_USINGLETON_H
#define CGRAPH_USINGLETON_H

#include "USingletonDefine.h"
#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T,
        USingletonType type = USingletonType::HUNGRY,
        CBool autoInit = false>
class USingleton : public UtilsObject {
public:
    explicit USingleton() noexcept {
        if (USingletonType::HUNGRY == type || autoInit) {
            /* 如果是饥汉模式，则直接构造 */
            create();
        }

        if (autoInit) {
            init();
        }
    }

    ~USingleton() override {
        clear();
    }

    /**
     * 获取singleton句柄信息
     * @return
     */
    T* get() {
        if (USingletonType::LAZY == type) {
            create();
        }

        T* handle = handle_;
        return handle;
    }

protected:
    CStatus init() override {
        CGRAPH_FUNCTION_BEGIN

        // 如果传入的是CObject类型的对象的话，则调用其init接口
        if (std::is_base_of<CObject, T>::value) {
            status = this->get()->init();
        }
        CGRAPH_FUNCTION_END
    }

    CStatus destroy() override {
        CGRAPH_FUNCTION_BEGIN
        if (std::is_base_of<CObject, T>::value) {
            status = this->get()->destroy();
        }
        CGRAPH_FUNCTION_END
    }

    /**
     * 单例的创建一个句柄
     * @return
     */
    CVoid create() {
        if (unlikely(nullptr == handle_)) {
            CGRAPH_LOCK_GUARD lock(lock_);
            if (nullptr == handle_) {
                handle_ = CGRAPH_SAFE_MALLOC_COBJECT(T)
            }
        }
    }

    /**
     * 销毁单例句柄
     * @return
     */
    CStatus clear() {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_LOCK_GUARD lock(lock_);
        CGRAPH_DELETE_PTR(handle_)
        CGRAPH_FUNCTION_END
    }

    CGRAPH_NO_ALLOWED_COPY(USingleton);

private:
    T* handle_ { nullptr };             // 对应的单例句柄信息
    std::mutex lock_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_USINGLETON_H
