/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UTaskWrapper.h
@Time: 2021/7/2 11:32 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTASKWRAPPER_H
#define CGRAPH_UTASKWRAPPER_H

#include <memory>

#include "../UThreadObject.h"


class UTaskWrapper : public UThreadObject {
    struct implBase {
        virtual void call() = 0;
        virtual ~implBase() = default;
    };

    std::unique_ptr<implBase> impl_;

    template<typename F>
    struct implType : implBase {
        F func_;
        explicit implType(F&& func) : func_(std::move(func)) {}
        void call() override { func_(); }
    };

public:
    template<typename F>
    UTaskWrapper(F&& f):
            impl_(new implType<F>(std::forward<F>(f))) {
    }

    void operator()() {
        if (impl_ != nullptr) {
            impl_->call();
        }
    }

    UTaskWrapper() = default;

    UTaskWrapper(UTaskWrapper&& task) noexcept:
            impl_(std::move(task.impl_)) {}

    UTaskWrapper &operator=(UTaskWrapper&& task) {
        impl_ = std::move(task.impl_);
        return *this;
    }

    UTaskWrapper(const UTaskWrapper &) = delete;
    UTaskWrapper(UTaskWrapper &) = delete;
    UTaskWrapper &operator=(const UTaskWrapper &) = delete;
};


using UFunctionWapperRef = UTaskWrapper &;
using UFunctionWapperPtr = UTaskWrapper *;

#endif //CGRAPH_UTASKWRAPPER_H
