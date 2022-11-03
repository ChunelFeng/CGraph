/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMessageUtils.h
@Time: 2022/11/1 22:48
@Desc: 
***************************/

#ifndef CGRAPH_GMESSAGEUTILS_H
#define CGRAPH_GMESSAGEUTILS_H

#include "GMessageManagerSingleton.h"

CGRAPH_NAMESPACE_BEGIN

/* 创建一个 topic */
#define CGRAPH_CREATE_MESSAGE_TOPIC(Type, topic, size)                              \
    CGraph::GMessageManagerSingletonPtr->createTopic<Type>(topic, size);            \

/* 删除一个 topic */
#define CGRAPH_REMOVE_MESSAGE_TOPIC(topic)                                          \
    CGraph::GMessageManagerSingletonPtr->removeTopic(topic);                        \

/* 发送一个 message param */
#define CGRAPH_PUB_MPARAM(Type, topic, value)                                       \
    CGraph::GMessageManagerSingletonPtr->pubTopicValue<Type>(topic, value);         \

/* 接收一个 message param */
#define CGRAPH_SUB_MPARAM(Type, topic, value)                                       \
    CGraph::GMessageManagerSingletonPtr->subTopicValue<Type>(topic, value);         \

/* 清空所有的topic */
#define CGRAPH_CLEAR_MESSAGES()                                                     \
    CGraph::GMessageManagerSingletonPtr->clear();                                   \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMESSAGEUTILS_H
