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
        init();
    }

    ~USingleton() override {
        deinit();
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

protected:
    CSTATUS init() final {
        CGRAPH_FUNCTION_BEGIN
        if (nullptr == handle_) {
            CGRAPH_LOCK_GUARD lock(lock_);
            if (nullptr == handle_) {
                handle_ = CGRAPH_SAFE_MALLOC_COBJECT(T);
                CGRAPH_ASSERT_NOT_NULL(handle_)
            }
        }

        CGRAPH_FUNCTION_END
    }

    CSTATUS deinit() final {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_LOCK_GUARD lock(lock_);
        CGRAPH_DELETE_PTR(handle_)
        CGRAPH_FUNCTION_END
    }

private:
    T* handle_ { nullptr };
    std::mutex lock_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_SINGLETONPROC_H
