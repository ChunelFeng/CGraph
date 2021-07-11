/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadBase.h
@Time: 2021/7/2 11:24 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTHREADBASE_H
#define CGRAPH_UTHREADBASE_H

#include <thread>

#include "../UThreadObject.h"
#include "../WorkStealingQueue/UWorkStealingQueue.h"
#include "../AtomicQueue/UAtomicQueue.h"

class UThreadBase : public UThreadObject {

protected:
    explicit UThreadBase() {
        done_ = true;
        is_init_ = false;
        is_running_ = false;
    }


    ~UThreadBase() override {
        deinit();    // 是不会重复deinit两次的，这样写虽然返回值非0，但是不会存在任何问题
    }


    /**
     * 所有线程类的deinit函数应该是一样的
     * 但是init函数不一样，因为线程构造函数不同
     * @return
     */
    CSTATUS deinit() override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(true)

        done_ = false;
        if (thread_.joinable()) {
            thread_.join();    // 等待线程结束
        }
        is_init_ = false;
        is_running_ = false;

        CGRAPH_FUNCTION_END
    }

protected:
    bool done_;                                               // 线程状态标记
    bool is_init_;                                            // 标记初始化状态
    bool is_running_;                                         // 是否正在执行
    std::thread thread_;                                      // 线程类
};

#endif //CGRAPH_UTHREADBASE_H
