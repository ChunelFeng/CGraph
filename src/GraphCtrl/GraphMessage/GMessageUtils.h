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

/* 创建一个 topic */
#define CGRAPH_CREATE_MESSAGE_TOPIC(Type, topic, size)                                 \
    CGraph::GMessageManagerSingleton.get()->createTopic<Type>(topic, size);            \

/* 删除一个 topic */
#define CGRAPH_REMOVE_MESSAGE_TOPIC(topic)                                             \
    CGraph::GMessageManagerSingleton.get()->removeTopic(topic);                        \

/* 发送一个 message param */
#define CGRAPH_PUB_MPARAM(Type, topic, value)                                          \
    CGraph::GMessageManagerSingleton.get()->pubTopicValue<Type>(topic, value);         \

/* 接收一个 message param */
#define CGRAPH_SUB_MPARAM(Type, topic, value)                                          \
    CGraph::GMessageManagerSingleton.get()->subTopicValue<Type>(topic, value);         \

/* 清空所有的topic */
#define CGRAPH_CLEAR_MESSAGES()                                                        \
    CGraph::GMessageManagerSingleton.get()->clear();                                   \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMESSAGEUTILS_H
