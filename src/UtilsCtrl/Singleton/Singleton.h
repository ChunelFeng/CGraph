/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: SingletonProc.h
@Time: 2021/6/11 7:06 下午
@Desc: 
***************************/

#ifndef CGRAPH_SINGLETON_H
#define CGRAPH_SINGLETON_H

#include <shared_mutex>
#include "../UtilsInclude.h"
#include "../../CObject/CObject.h"

template<typename T>
class Singleton : public CObject {
public:
    explicit Singleton() {
        create();
    }

    ~Singleton() override {
        destroy();
    }

    T* get() {
        CGRAPH_RLOCK lock(lock_);
        return handle_;
    }

protected:
    CSTATUS create() {
        CGRAPH_FUNCTION_BEGIN
        if (nullptr == handle_) {
            CGRAPH_WLOCK lock(lock_);
            if (nullptr == handle_) {
                handle_ = new T();
            }
        }

        CGRAPH_FUNCTION_END
    }

    CSTATUS run() {
        CGRAPH_NO_SUPPORT
    }

    CSTATUS destroy() {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_WLOCK lock(lock_);
        CGRAPH_DELETE_PTR(handle_)
        CGRAPH_FUNCTION_END
    }

private:
    T* handle_ { nullptr };
    std::shared_mutex lock_;
};



#endif //CGRAPH_SINGLETONPROC_H
