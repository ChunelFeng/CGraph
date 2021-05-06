/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphThreadPool.h
@Time: 2021/5/1 3:52 下午
@Desc:
***************************/

#ifndef CGRAPH_GRAPHTHREADPOOL_H
#define CGRAPH_GRAPHTHREADPOOL_H

#include <vector>
#include <queue>
#include <atomic>
#include <future>
#include <condition_variable>

#include "../GraphDefine.h"
#include "../GraphNode/GraphNode.h"


class GraphThreadPool
{
private:
    std::vector<std::thread> pool_;                   // 线程池
    std::queue<TaskFunc> tasks_que_;             // 任务队列
    std::mutex mtx_;                             // 同步
    std::condition_variable task_cond_;          // 条件阻塞
    std::atomic<bool> run_{ true };              // 线程池是否执行
    std::atomic<int> idl_thd_num_{ 0 };          // 空闲线程数量
    std::atomic<int> max_thd_num_{MAX_THREAD_NUM};    // 最大线程数

public:
    explicit GraphThreadPool(int maxThdNum = MAX_THREAD_NUM) {
        max_thd_num_ = maxThdNum;
        addThread(INIT_THREAD_NUM);
    }

    virtual ~GraphThreadPool() {
        run_ = false;
        task_cond_.notify_all();     // 唤醒所有线程执行
        for (std::thread& thd : pool_) {
            if(thd.joinable())
                thd.join();     // 等待任务结束， 前提：线程一定会执行完
        }
    }

    std::future<int> commit(GraphNode* nodes) {
        auto curTask = std::make_shared<std::packaged_task<int()>>(std::bind(&GraphNode::process, nodes));
        std::future<int> future = curTask->get_future();
        {
            // 添加任务到队列
            std::lock_guard<std::mutex> lock{ mtx_ };
            tasks_que_.push([curTask]() {
                (*curTask)();
            });
        }

        if (idl_thd_num_ < 1 && pool_.size() < max_thd_num_) {
            // 如果空闲thread小于1，并且不超过max限制
            addThread(1);
        }

        task_cond_.notify_one();
        return future;
    }

    void run() {
        while (run_) {
            TaskFunc curFunc = nullptr;
            {
                std::unique_lock<std::mutex> lock{ mtx_ };
                task_cond_.wait(lock, [this]{
                    // 值为false的时候，会阻塞当前线程
                    // 收到其他线程通知后，值为true的时候，会解除阻塞
                    return (!run_ || !tasks_que_.empty());
                });

                if (!run_ && tasks_que_.empty()) {
                    return;
                }

                curFunc = std::move(tasks_que_.front());    // 按先进先出从队列取一个 task
                tasks_que_.pop();
            }

            idl_thd_num_--;
            curFunc();
            idl_thd_num_++;
        }
    }

protected:
    /**
     * 添加执行线程
     * @param size
     */
    void addThread(int size) {
        for (; pool_.size() < max_thd_num_ && size > 0; --size){
            pool_.emplace_back(std::thread(&GraphThreadPool::run, this));
            // 如果线程执行函数退出，则空闲线程数+1
            idl_thd_num_++;
        }
    }
};


#endif //CGRAPH_GRAPHTHREADPOOL_H
