/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPool.h
@Time: 2021/7/4 1:34 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTHREADPOOL_H
#define CGRAPH_UTHREADPOOL_H

#include <vector>
#include <future>
#include <thread>

#include "UThreadObject.h"
#include "AtomicQueue/UAtomicQueue.h"
#include "Thread/UThread.h"


class UThreadPool : public UThreadObject {

public:
    explicit UThreadPool() {
        cur_index_ = 0;
        for (int i = 0; i < DEFAULT_THREAD_SIZE; ++i) {
            auto ptr = new(std::nothrow) UThread();
            ptr->setThreadPoolInfo(i, &this->task_queue_, &this->threads_);
            threads_.emplace_back(ptr);
        }
    }

    ~UThreadPool() override {
        for (auto thread : threads_) {
            CGRAPH_DELETE_PTR(thread);
        }
    }

    /**
     * 提交任务信息
     * @tparam FunctionType
     * @param func
     * @return
     */
    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> commit(FunctionType func) {
        typedef typename std::result_of<FunctionType()>::type resultType;

        std::packaged_task<resultType()> task(func);
        std::future<resultType> result(task.get_future());

        if (++cur_index_ == DEFAULT_THREAD_SIZE) {
            cur_index_ = 0;
        }
        threads_[cur_index_]->work_stealing_queue_.push(std::move(task));
        return result;
    }

private:
    int cur_index_;                                   // 记录放入的线程数
    UAtomicQueue<UTaskWrapper> task_queue_;           // 用于存放普通任务
    std::vector<UThreadPtr> threads_;                 // 记录所有的线程
};

using UThreadPoolPtr = UThreadPool *;
using UThreadPoolRef = UThreadPool &;


#endif //CGRAPH_UTHREADPOOL_H
