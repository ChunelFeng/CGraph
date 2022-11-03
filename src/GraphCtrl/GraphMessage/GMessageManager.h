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
#include <unordered_map>

#include "GMessage.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T = GMessageParam,
        std::enable_if_t<std::is_base_of<GMessageParam, T>::value, int> = 0>
class GMessageManager : public GMessageObject, public GraphManager<T> {
public:
    /**
     * 创建 topic
     * @tparam TImpl
     * @param topic
     * @param size
     * @return
     */
    template<typename TImpl,
            std::enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus createTopic(const std::string& topic, CUint size) {
        CGRAPH_FUNCTION_BEGIN

        auto result = message_map_.find(topic);
        if (result != message_map_.end()) {
            // 如果类型和size完全匹配的话，则直接返回创建成功。否则返回错误
            auto curTopic = result->second;
            status = (typeid(*curTopic).name() == typeid(GMessage<TImpl>).name() && curTopic->getCapacity() == size)
                     ? CStatus() : CStatus("create topic duplicate");
        } else {
            // 创建一个 topic信息
            auto message = CGRAPH_SAFE_MALLOC_COBJECT(GMessage<TImpl>);
            message->setCapacity(size);
            message_map_.insert(std::pair<const std::string&, GMessagePtr<T> >(topic, GMessagePtr<T>(message)));
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
        auto result = message_map_.find(topic);
        if (result == message_map_.end()) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + topic + "] topic");
        }

        CGRAPH_DELETE_PTR(result->second);
        message_map_.erase(result);

        CGRAPH_FUNCTION_END
    }

    /**
     * 根据传入的topic，获得信息
     * @tparam TImpl
     * @param topic
     * @param value
     * @return
     */
    template<typename TImpl,
            std::enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus subTopicValue(const std::string& topic, TImpl& value) {
        CGRAPH_FUNCTION_BEGIN
        auto result = message_map_.find(topic);
        if (result == message_map_.end()) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + topic + "] topic");
        }

        auto message = (GMessagePtr<TImpl>)(result->second);
        CGRAPH_ASSERT_NOT_NULL(message);

        message->sub(value);
        CGRAPH_FUNCTION_END
    }

    /**
     * 根据传入的topic，输入信息
     * @tparam TImpl
     * @param topic
     * @param value
     * @return
     */
    template<typename TImpl,
            std::enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus pubTopicValue(const std::string& topic, const TImpl& value) {
        CGRAPH_FUNCTION_BEGIN
        auto result = message_map_.find(topic);
        if (result == message_map_.end()) {
            CGRAPH_RETURN_ERROR_STATUS("no find [" + topic + "] topic");
        }

        auto message = static_cast<GMessagePtr<T> >(result->second);
        CGRAPH_ASSERT_NOT_NULL(message);

        message->pub(value);
        CGRAPH_FUNCTION_END
    }

    /**
     * 清空数据
     * @return
     */
    CStatus clear() override {
        CGRAPH_FUNCTION_BEGIN
        for (auto& cur : message_map_) {
            CGRAPH_DELETE_PTR(cur.second)
        }
        message_map_.clear();
        CGRAPH_FUNCTION_END
    }


protected:
    CStatus init() override {
        /**
         * 仅可能被 USingleton 类调用，自动构造时候使用
         * 防止菱形引用的奇异
         */
        CGRAPH_EMPTY_FUNCTION
    }

    CStatus destroy() override {
        CGRAPH_EMPTY_FUNCTION
    }


private:
    std::unordered_map<std::string, GMessagePtr<T> > message_map_;    // 记录 topic 和 message queue 信息

    template<typename U, USingletonType, CBool> friend class USingleton;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMESSAGEMANAGER_H
