/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: SingletonProc.h
@Time: 2021/6/11 7:06 下午
@Desc: 
***************************/

#ifndef CGRAPH_USINGLETON_H
#define CGRAPH_USINGLETON_H

#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T>
class USingleton : public UtilsObject {
public:
    explicit USingleton() {
        create();
    }

    ~USingleton() override {
        destroy();
    }

    /**
     * 获取singleton句柄信息
     * @return
     */
    T* get() {
        CGRAPH_LOCK_GUARD lock(lock_);
        T* handle = handle_;
        return handle;
    }

    CSTATUS init() override {
        CGRAPH_FUNCTION_BEGIN

        // 如果传入的是CObject类型的对象的话，则调用其init接口
        if constexpr (std::is_base_of_v<CObject, T>) {
            status = this->get()->init();
        }
        CGRAPH_FUNCTION_END
    }

    CSTATUS deinit() override {
        CGRAPH_FUNCTION_BEGIN
        if constexpr (std::is_base_of_v<CObject, T>) {
            status = this->get()->deinit();
        }
        CGRAPH_FUNCTION_END
    }

protected:
    void create() {
        if (nullptr == handle_) {
            CGRAPH_LOCK_GUARD lock(lock_);
            if (nullptr == handle_) {
                handle_ = CGRAPH_SAFE_MALLOC_COBJECT(T);
            }
        }
    }

    void destroy() {
        CGRAPH_LOCK_GUARD lock(lock_);
        CGRAPH_DELETE_PTR(handle_)
    }

private:
    T* handle_ { nullptr };
    std::mutex lock_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_SINGLETONPROC_H
