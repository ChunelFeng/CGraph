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
#include <utility>
#include <type_traits>

#include "../UThreadObject.h"

CGRAPH_NAMESPACE_BEGIN

class UTask : public CStruct {
    struct TaskBased {
        explicit TaskBased() = default;
        virtual CVoid call() = 0;
        virtual ~TaskBased() = default;
    };

    // 退化以获得实际类型，修改思路参考：https://github.com/ChunelFeng/CThreadPool/pull/3
    template<typename F, typename T = typename std::decay<F>::type>
    struct TaskDerided : TaskBased {
        T func_ {};
        explicit TaskDerided(F&& func) : func_(std::forward<F>(func)) {}
        CVoid call() final { func_(); }
    };

public:
    template<typename F,
        typename std::enable_if<!std::is_same<typename std::decay<F>::type, UTask>::value, int>::type = 0>
    explicit UTask(F&& func, const CBool owner = true)
        : impl_(new TaskDerided<F>(std::forward<F>(func))),
          owner_(owner) {
    }

    CVoid operator()() const {
        if (likely(impl_)) {
            impl_->call();
        }
    }

    explicit UTask() = default;

    explicit UTask(const UTask* task) {
        if (likely(task)) {
            impl_ = task->impl_;
            owner_ = false;
        }
    }

    explicit UTask(UTask* task) {
        if (likely(task)) {
            impl_ = task->impl_;
            owner_ = false;
        }
    }

    ~UTask() override {
        if (owner_) {
            CGRAPH_DELETE_PTR(impl_);
        }
    }

    UTask(UTask&& task) noexcept:
            impl_(task.impl_),
            owner_(task.owner_) {
        task.impl_ = nullptr;
        task.owner_ = false;
    }

    UTask& operator=(UTask&& task) noexcept {
        if (this != &task) {
            if (owner_) {
                CGRAPH_DELETE_PTR(impl_);
            }

            impl_ = task.impl_;
            owner_ = task.owner_;

            task.impl_ = nullptr;
            task.owner_ = false;
        }

        return *this;
    }

    CGRAPH_NO_ALLOWED_COPY(UTask)

private:
    TaskBased* impl_ { nullptr };
    CBool owner_ { true };                                // impl_ 是否归属当前对象
};


using UTaskRef = UTask &;
using UTaskPtr = UTask *;
using UTaskArr = std::vector<UTask>;
using UTaskArrRef = std::vector<UTask> &;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTASK_H
