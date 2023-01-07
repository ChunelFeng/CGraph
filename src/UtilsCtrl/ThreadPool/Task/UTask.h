/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UTask.h
@Time: 2021/7/2 11:32 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTASK_H
#define CGRAPH_UTASK_H

#include <vector>
#include <memory>

#include "../UThreadObject.h"

CGRAPH_NAMESPACE_BEGIN

class UTask : public UThreadObject {
    struct taskBased {
        explicit taskBased() = default;
        virtual CVoid call() = 0;
        virtual ~taskBased() = default;
    };

    template<typename F>
    struct taskDerided : taskBased {
        F func_;
        explicit taskDerided(F&& func) : func_(std::move(func)) {}
        CVoid call() override { func_(); }
    };

public:
    template<typename F>
    UTask(F&& f, int priority = 0)
        : impl_(new taskDerided<F>(std::forward<F>(f)))
        , priority_(priority) {}

    CVoid operator()() {
        impl_->call();
    }

    UTask() = default;

    UTask(UTask&& task) noexcept:
            impl_(std::move(task.impl_)),
            priority_(task.priority_) {}

    UTask &operator=(UTask&& task) noexcept {
        impl_ = std::move(task.impl_);
        priority_ = task.priority_;
        return *this;
    }

    CBool operator>(const UTask& task) const {
        return priority_ < task.priority_;    // 新加入的，放到后面
    }

    CBool operator<(const UTask& task) const {
        return priority_ >= task.priority_;
    }

    CGRAPH_NO_ALLOWED_COPY(UTask)

private:
    std::unique_ptr<taskBased> impl_ = nullptr;
    int priority_ = 0;                                 // 任务的优先级信息
};


using UTaskRef = UTask &;
using UTaskPtr = UTask *;
using UTaskArr = std::vector<UTask>;
using UTaskArrRef = std::vector<UTask> &;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTASK_H
