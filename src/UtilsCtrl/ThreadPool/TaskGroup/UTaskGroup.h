/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UTaskGroup.h
@Time: 2022/1/2 2:17 下午
@Desc: 任务组，用于批量提交
***************************/

#ifndef CGRAPH_UTASKGROUP_H
#define CGRAPH_UTASKGROUP_H

#include "../UThreadObject.h"

CGRAPH_NAMESPACE_BEGIN

class UTaskGroup : public UThreadObject {
public:
    /**
     * 添加一个任务
     * @param task
     */
    void addTask(const CGRAPH_DEFAULT_FUNCTION &task) {
        task_arr_.emplace_back(task);
    }

    /**
     * 设置任务最大超时时间
     * @param ttl
     */
    void setTtlMs(int ttl) {
        this->ttl_ms_ = ttl;
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
    int getSize() {
        int size = (int) task_arr_.size();
        return size;
    }

private:
    std::vector<CGRAPH_DEFAULT_FUNCTION> task_arr_;
    int ttl_ms_ = CGRAPH_DEFAULT_GROUP_TTL_MS;                          // 任务组最大执行耗时

    friend class UThreadPool;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTASKGROUP_H
