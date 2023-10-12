/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMessageManager.h
@Time: 2022/10/30 20:48
@Desc: 信息管理类
***************************/

#ifndef CGRAPH_GMESSAGEMANAGER_H
#define CGRAPH_GMESSAGEMANAGER_H

#include <string>
#include <set>
#include <unordered_map>
#include <memory>

#include "GMessage.h"
#include "GMessageDefine.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T = GMessageParam,
        c_enable_if_t<std::is_base_of<GMessageParam, T>::value, int> = 0>
class GMessageManager : public GMessageObject,
                        public GraphManager<GMessage<T> > {
public:
    /**
     * 创建 topic
     * @tparam TImpl
     * @param topic
     * @param size
     * @return
     */
    template<typename TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus createTopic(const std::string& topic, CUint size) {
        CGRAPH_FUNCTION_BEGIN

        auto innerTopic = SEND_RECV_PREFIX + topic;    // 中间做一层映射，用来区分是 PubSub的，还是SendRecv的
        auto result = send_recv_message_map_.find(innerTopic);
        if (result != send_recv_message_map_.end()) {
            // 如果类型和size完全匹配的话，则直接返回创建成功。否则返回错误
            auto curTopic = result->second;
            status = (typeid(*curTopic).name() == typeid(GMessage<TImpl>).name() && curTopic->getCapacity() == size)
                     ? CStatus() : CErrStatus("create topic [" + topic + "] duplicate");
        } else {
            // 创建一个 topic信息
            auto message = UAllocator::safeMallocTemplateCObject<GMessage<TImpl>, CUint>(size);
            send_recv_message_map_.insert(std::pair<const std::string&, GMessagePtr<T> >(innerTopic, GMessagePtr<T>(message)));
        }

        CGRAPH_FUNCTION_END
    }

    /**
     * 删除 topic
     * @param topic
     * @return
     */
    CStatus removeTopic(const std::string& topic) {
        CGRAPH_FUNCTION_BEGIN
        auto innerTopic = SEND_RECV_PREFIX + topic;
        auto result = send_recv_message_map_.find(innerTopic);
        if (result == send_recv_message_map_.end()) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + topic + "] topic");
        }

        CGRAPH_DELETE_PTR(result->second);
        send_recv_message_map_.erase(result);

        CGRAPH_FUNCTION_END
    }

    /**
     * 根据传入的topic，获得信息
     * @tparam TImpl
     * @param topic
     * @param value
     * @param timeout
     * @return
     */
    template<typename TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus recvTopicValue(const std::string& topic,
                           TImpl& value,
                           CMSec timeout = CGRAPH_MAX_BLOCK_TTL) {
        CGRAPH_FUNCTION_BEGIN
        auto innerTopic = SEND_RECV_PREFIX + topic;
        auto result = send_recv_message_map_.find(innerTopic);
        if (result == send_recv_message_map_.end()) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + topic + "] topic");
        }

        auto message = (GMessagePtr<TImpl>)(result->second);
        CGRAPH_ASSERT_NOT_NULL(message);

        status = message->recv(value, timeout);
        CGRAPH_FUNCTION_END
    }

    /**
     * 根据传入的topic，获得信息。仅针对传入智能指针的情况
     * @tparam TImpl
     * @param topic
     * @param value
     * @param timeout
     * @return
     * @notice 这里的逻辑，跟上面的函数一致。里面调用了底层RingBuffer的同名不同入参的接口。
     *         本人暂时没有能力完成接口的统一。如果有了解这一块内容的朋友，欢迎交流指正。
     */
    template<typename TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus recvTopicValue(const std::string& topic,
                           std::unique_ptr<TImpl>& value,
                           CMSec timeout = CGRAPH_MAX_BLOCK_TTL) {
        CGRAPH_FUNCTION_BEGIN
        auto innerTopic = SEND_RECV_PREFIX + topic;
        auto result = send_recv_message_map_.find(innerTopic);
        if (result == send_recv_message_map_.end()) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + topic + "] topic");
        }

        auto message = (GMessagePtr<TImpl>)(result->second);
        CGRAPH_ASSERT_NOT_NULL(message);

        status = message->recv(value, timeout);
        CGRAPH_FUNCTION_END
    }

    /**
     * 根据传入的topic，输入信息
     * @tparam TImpl
     * @param topic
     * @param value
     * @param strategy
     * @return
     */
    template<typename TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus sendTopicValue(const std::string& topic,
                           const TImpl& value,
                           GMessagePushStrategy strategy) {
        CGRAPH_FUNCTION_BEGIN
        auto innerTopic = SEND_RECV_PREFIX + topic;
        auto result = send_recv_message_map_.find(innerTopic);
        if (result == send_recv_message_map_.end()) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + topic + "] topic");
        }

        auto message = static_cast<GMessagePtr<T> >(result->second);
        CGRAPH_ASSERT_NOT_NULL(message);

        message->send(value, strategy);
        CGRAPH_FUNCTION_END
    }

    /**
     * 根据传入的topic，输入智能指针类型的信息
     * @tparam TImpl
     * @param topic
     * @param value
     * @param strategy
     * @return
     */
    template<typename TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus sendTopicValue(const std::string& topic,
                           std::unique_ptr<TImpl>& value,
                           GMessagePushStrategy strategy) {
        CGRAPH_FUNCTION_BEGIN
        auto innerTopic = SEND_RECV_PREFIX + topic;
        auto result = send_recv_message_map_.find(innerTopic);
        if (result == send_recv_message_map_.end()) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + topic + "] topic");
        }

        auto message = static_cast<GMessagePtr<T> >(result->second);
        CGRAPH_ASSERT_NOT_NULL(message);

        message->send(value, strategy);
        CGRAPH_FUNCTION_END
    }

    /**
     * 绑定对应的topic信息，并且获取 conn_id 信息
     * @tparam TImpl
     * @param topic
     * @param size
     * @return
     */
    template<typename TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CIndex bindTopic(const std::string& topic, CUint size) {
        auto innerTopic = PUB_SUB_PREFIX + topic;
        auto message = UAllocator::safeMallocTemplateCObject<GMessage<TImpl>, CUint>(size);

        CGRAPH_LOCK_GUARD lock(bind_mutex_);
        CIndex connId = (++cur_conn_id_);
        auto result = pub_sub_message_map_.find(innerTopic);
        if (result != pub_sub_message_map_.end()) {
            // 如果之前有的话，则在后面添加一个
            auto& messageSet = result->second;
            messageSet.insert((GMessagePtr<T>)message);
        } else {
            // 如果是这个topic第一次被绑定，则创建一个对应的set信息
            std::set<GMessagePtr<T>> messageSet;
            messageSet.insert((GMessagePtr<T>)message);
            pub_sub_message_map_[innerTopic] = messageSet;
        }
        conn_message_map_[connId] = (GMessagePtr<T>)message;
        return connId;
    }

    /**
     * 开始发送对应topic的信息
     * @tparam TImpl
     * @param topic
     * @param value
     * @param strategy
     * @return
     */
    template<typename TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus pubTopicValue(const std::string& topic,
                          const TImpl& value,
                          GMessagePushStrategy strategy) {
        CGRAPH_FUNCTION_BEGIN
        auto innerTopic = PUB_SUB_PREFIX + topic;
        auto result = pub_sub_message_map_.find(innerTopic);
        if (result == pub_sub_message_map_.end()) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + topic + "] topic");
        }

        auto& messageSet = result->second;
        for (auto msg : messageSet) {
            msg->send(value, strategy);    // 给所有订阅的信息，一次发送消息
        }
        CGRAPH_FUNCTION_END
    }

    /**
     * 根据传入的 connId信息，来获取对应的message信息
     * @tparam TImpl
     * @param connId
     * @param value
     * @param timeout
     * @return
     */
    template<typename TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus subTopicValue(CIndex connId, TImpl& value, CMSec timeout = CGRAPH_MAX_BLOCK_TTL) {
        CGRAPH_FUNCTION_BEGIN
        if (conn_message_map_.end() == conn_message_map_.find(connId)) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + std::to_string(connId) + "] connect");
        }

        auto message = (GMessagePtr<TImpl>)(conn_message_map_[connId]);
        status = message->recv(value, timeout);
        CGRAPH_FUNCTION_END
    }

    /**
     * 根据传入的 connId信息，来获取对应的message信息。仅针对传入智能指针的情况
     * @tparam TImpl
     * @param connId
     * @param value
     * @param timeout
     * @return
     */
    template<typename TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus subTopicValue(CIndex connId, std::unique_ptr<TImpl>& value, CMSec timeout = CGRAPH_MAX_BLOCK_TTL) {
        CGRAPH_FUNCTION_BEGIN
        if (conn_message_map_.end() == conn_message_map_.find(connId)) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + std::to_string(connId) + "] connect");
        }

        auto message = (GMessagePtr<TImpl>)(conn_message_map_[connId]);
        status = message->recv(value, timeout);
        CGRAPH_FUNCTION_END
    }

    /**
     * 删除对应的topic信息
     * @param topic
     * @return
     */
    CStatus dropTopic(const std::string& topic) {
        CGRAPH_FUNCTION_BEGIN
        auto innerTopic = PUB_SUB_PREFIX + topic;
        auto result = pub_sub_message_map_.find(innerTopic);
        if (result == pub_sub_message_map_.end()) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + topic + "] topic");
        }

        auto& messageSet = result->second;
        for (auto msg : messageSet) {
            CGRAPH_DELETE_PTR(msg)
        }
        pub_sub_message_map_.erase(innerTopic);
        CGRAPH_FUNCTION_END
    }

    /**
     * 清空数据
     * @return
     */
    CStatus clear() final {
        CGRAPH_FUNCTION_BEGIN
        for (auto& cur : send_recv_message_map_) {
            CGRAPH_DELETE_PTR(cur.second)
        }

        for (auto& cur : pub_sub_message_map_) {
            for (auto iter : cur.second) {
                CGRAPH_DELETE_PTR(iter);
            }
        }
        send_recv_message_map_.clear();
        pub_sub_message_map_.clear();
        cur_conn_id_ = 0;
        CGRAPH_FUNCTION_END
    }


protected:
    CStatus init() final {
        /**
         * 仅可能被 USingleton 类调用，自动构造时候使用
         * 防止菱形引用的奇异
         */
        CGRAPH_EMPTY_FUNCTION
    }

    CStatus destroy() final {
        CGRAPH_EMPTY_FUNCTION
    }

    ~GMessageManager() override {
        clear();    // 释放所有的信息
    }

private:
    std::unordered_map<std::string, GMessagePtr<T>> send_recv_message_map_;    // 记录 topic 和 message queue 信息
    std::unordered_map<std::string, std::set<GMessagePtr<T>>> pub_sub_message_map_;    // 记录 pub和sub的 message 的信息
    std::unordered_map<CIndex, GMessagePtr<T>> conn_message_map_;    // 用于根据 index反推message信息
    CIndex cur_conn_id_ = 0;    // 记录当前的conn信息

    std::mutex bind_mutex_;

    template<typename U, USingletonType, CBool> friend class USingleton;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMESSAGEMANAGER_H
