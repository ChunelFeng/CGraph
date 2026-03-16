/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMessage.h
@Time: 2022/10/30 20:38
@Desc: 
***************************/

#ifndef CGRAPH_GMESSAGE_H
#define CGRAPH_GMESSAGE_H

#include <memory>

#include "GMessageObject.h"
#include "GMessageDefine.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T, CUInt capacity = CGRAPH_DEFAULT_RINGBUFFER_SIZE,
        c_enable_if_t<std::is_base_of<GMessageParam, T>::value, int> = 0>
class GMessage : public GMessageObject {
public:
    explicit GMessage(CUInt size = capacity,
        const CIndex connId = 0) {
        queue_.setCapacity(size);
        conn_id_ = connId;
    }

    /**
     * 析构函数。释放前，要先释放队列中所有的信息
     */
    ~GMessage() override {
        queue_.clear();
    }

    /**
     * 写入智能指针类型的参数
     * @tparam TImpl
     * @param value
     * @param strategy
     * @return
     */
    template<class TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CVoid send(const std::shared_ptr<TImpl>& value, GMessagePushStrategy strategy) {
        queue_.push(value, strategy);
    }

    /**
     * 通过智能指针的方式传递
     * @tparam TImpl
     * @param value
     * @param timeout
     * @return
     */
    template<class TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus recv(std::shared_ptr<TImpl>& value, CMSec timeout) {
        return queue_.waitPopWithTimeout(value, timeout);
    }

    /**
     * 获取容量大小
     * @return
     */
    CUInt getCapacity() const {
        return queue_.getCapacity();
    }

    /**
     * 获取 connId 信息
     * @return
     */
    CIndex getConnId() const {
        return conn_id_;
    }

private:
    UAtomicRingBufferQueue<T, capacity> queue_ {};
    CIndex conn_id_ {0};                            // 用于记录message的唯一标识，用于 pub-sub场景
};


template<typename T, CUInt capacity = CGRAPH_DEFAULT_RINGBUFFER_SIZE>
using GMessagePtr = GMessage<T, capacity> *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMESSAGE_H
