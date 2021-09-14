/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UTaskWrapper.h
@Time: 2021/7/2 11:32 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTASKWRAPPER_H
#define CGRAPH_UTASKWRAPPER_H

#include <vector>
#include <memory>

#include "../UThreadObject.h"


class UTaskWrapper : public UThreadObject {
    struct implBase {
        virtual void call() = 0;
        virtual ~implBase() = default;
    };

    template<typename F>
    struct implDeride : implBase {
        F func_;
        explicit implDeride(F&& func) : func_(std::move(func)) {}
        void call() override { func_(); }
    };

    std::unique_ptr<implBase> impl_ = nullptr;

public:
    template<typename F>
    UTaskWrapper(F&& f) : impl_(new implDeride<F>(std::forward<F>(f))) {
    }

    void operator()() {
        if (unlikely(impl_ != nullptr)) {
            impl_->call();
        }
    }

    UTaskWrapper() = default;

    UTaskWrapper(UTaskWrapper&& task) noexcept:
            impl_(std::move(task.impl_)) {}

    UTaskWrapper &operator=(UTaskWrapper&& task) noexcept {
        impl_ = std::move(task.impl_);
        return *this;
    }

    UTaskWrapper(const UTaskWrapper &) = delete;
    UTaskWrapper(UTaskWrapper &) = delete;
    UTaskWrapper &operator=(const UTaskWrapper &) = delete;
};


using UTaskWrapperRef = UTaskWrapper &;
using UTaskWrapperPtr = UTaskWrapper *;
using UTaskWrapperArr = std::vector<UTaskWrapper>;

#endif //CGRAPH_UTASKWRAPPER_H
