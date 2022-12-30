/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMessageUtils.h
@Time: 2022/11/1 22:48
@Desc: 
***************************/

#ifndef CGRAPH_GMESSAGEUTILS_H
#define CGRAPH_GMESSAGEUTILS_H

#include "GMessageManager.h"

CGRAPH_NAMESPACE_BEGIN

/*******************************
 * 以下是 send recv 的功能对应的接口
 *******************************/
/* 创建一个 topic */
#define CGRAPH_CREATE_MESSAGE_TOPIC(Type, topic, size)                                 \
    CGraph::GMessageManagerSingleton.get()->createTopic<Type>(topic, size);            \

/* 删除一个 topic */
#define CGRAPH_REMOVE_MESSAGE_TOPIC(topic)                                             \
    CGraph::GMessageManagerSingleton.get()->removeTopic(topic);                        \

/* 发送一个 message param */
#define CGRAPH_SEND_MPARAM(Type, topic, value)                                         \
    CGraph::GMessageManagerSingleton.get()->sendTopicValue<Type>(topic, value);        \

/* 接收一个 message param */
#define CGRAPH_RECV_MPARAM(Type, topic, value)                                         \
    CGraph::GMessageManagerSingleton.get()->recvTopicValue<Type>(topic, value);        \


/*******************************
 * 以下是 pub sub 的功能对应的接口
 *******************************/
/* 绑定一个topic，并且返回 conn id 信息 */
#define CGRAPH_BIND_MESSAGE_TOPIC(Type, topic, size)                                   \
    CGraph::GMessageManagerSingleton.get()->bindTopic<Type>(topic, size);              \

/* 发布一个消息 */
#define CGRAPH_PUB_MPARAM(Type, topic, value)                                          \
    CGraph::GMessageManagerSingleton.get()->pubTopicValue<Type>(topic, value);         \

/* 订阅一个消息 */
#define CGRAPH_SUB_MPARAM(Type, connId, value)                                         \
    CGraph::GMessageManagerSingleton.get()->subTopicValue<Type>(connId, value);        \

/* 删除消息 */
#define CGRAPH_DROP_MESSAGE_TOPIC(topic)                                               \
    CGraph::GMessageManagerSingleton.get()->dropTopic(topic);                          \


/****************************
 * 清空所有的topic
 ****************************/
/* 清空所有的topic */
#define CGRAPH_CLEAR_MESSAGES()                                                        \
    CGraph::GMessageManagerSingleton.get()->clear();                                   \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMESSAGEUTILS_H
