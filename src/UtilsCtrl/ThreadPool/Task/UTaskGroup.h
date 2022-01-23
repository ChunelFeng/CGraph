/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UTaskGroup.h
@Time: 2022/1/2 2:17 下午
@Desc: 任务组，用于批量提交
***************************/

#ifndef CGRAPH_UTASKGROUP_H
#define CGRAPH_UTASKGROUP_H

#include <climits>
#include <utility>

#include "../UThreadObject.h"

CGRAPH_NAMESPACE_BEGIN

class UTaskGroup : public UThreadObject {
public:
    explicit UTaskGroup() = default;

    /**
     * 直接通过函数来申明taskGroup
     * @param task
     * @param ttlMs
     * @param onFinished
     */
    explicit UTaskGroup(const CGRAPH_DEFAULT_FUNCTION& task,
                        int ttlMs = INT_MAX,
                        const CGRAPH_CALLBACK_FUNCTION& onFinished = nullptr) noexcept {
        this->addTask(task)
            ->setTtlMs(ttlMs)
            ->setOnFinished(onFinished);
    }


    /**
     * 添加一个任务
     * @param task
     */
    UTaskGroup* addTask(const CGRAPH_DEFAULT_FUNCTION &task) {
        task_arr_.emplace_back(task);
        return this;
    }


    /**
     * 设置任务最大超时时间
     * @param ttl
     */
    UTaskGroup* setTtlMs(int ttlMs) {
        this->ttl_ms_ = ttlMs > 0 ? ttlMs : INT_MAX;
        return this;
    }


    /**
     * 设置执行完成后的回调函数
     * @param onFinished
     * @return
     */
    UTaskGroup* setOnFinished(const CGRAPH_CALLBACK_FUNCTION& onFinished) {
        this->on_finished_ = onFinished;
        return this;
    }


    /**
     * 获取最大超时时间信息
     * @return
     */
    [[nodiscard]] int getTtlMs() const {
        return this->ttl_ms_;
    }


    /**
     * 清空任务组
     */
    void clear() {
        task_arr_.clear();
    }


    /**
     * 获取任务组大小
     * @return
     */
    [[nodiscard]] int getSize() const {
        int size = (int) task_arr_.size();
        return size;
    }

private:
    std::vector<CGRAPH_DEFAULT_FUNCTION> task_arr_;         // 任务消息
    int ttl_ms_ = INT_MAX;                                  // 任务组最大执行耗时
    CGRAPH_CALLBACK_FUNCTION on_finished_ = nullptr;        // 执行函数任务结束

    friend class UThreadPool;
};

using UTaskGroupPtr = UTaskGroup *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTASKGROUP_H
