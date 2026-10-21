/***************************
@Author: Chunel
@File: PywGMessage.h
@Desc: Python message ownership and bindings for the C++ message manager
***************************/

#ifndef CGRAPH_PYWGMESSAGE_H
#define CGRAPH_PYWGMESSAGE_H

#include <pybind11/pybind11.h>

#include "CGraph.h"


class PywGMessage {
public:
    static CStatus createTopic(const std::string& topic,
                               const pybind11::object& /* messageType */,
                               CUInt capacity) {
        pybind11::gil_scoped_release gsr;
        return manager()->createTopic<CGraph::GMessageParam>(topic, capacity);
    }

    static CStatus removeTopic(const std::string& topic) {
        pybind11::gil_scoped_release gsr;
        return manager()->removeTopic(topic);
    }

    static CStatus sendMessage(const std::string& topic,
                               const std::shared_ptr<CGraph::GMessageParam>& message,
                               CGraph::GMessagePushStrategy strategy) {
        pybind11::gil_scoped_release gsr;
        return manager()->sendTopicValue(topic, message, strategy);
    }

    static pybind11::tuple recvMessage(const std::string& topic, CMSec timeout) {
        std::shared_ptr<CGraph::GMessageParam> value;
        CStatus status;
        {
            pybind11::gil_scoped_release gsr;
            status = manager()->recvTopicValue(topic, value, timeout);
        }
        return pybind11::make_tuple(status, value);
    }

    static CIndex bindTopic(const std::string& topic,
                            const pybind11::object& /* messageType */,
                            CUInt capacity) {
        pybind11::gil_scoped_release gsr;
        return manager()->bindTopic<CGraph::GMessageParam>(topic, capacity);
    }

    static CStatus pubMessage(const std::string& topic,
                              const std::shared_ptr<CGraph::GMessageParam>& message,
                              CGraph::GMessagePushStrategy strategy) {
        pybind11::gil_scoped_release gsr;
        return manager()->pubTopicValue(topic, message, strategy);
    }

    static pybind11::tuple subMessage(CIndex connId, CMSec timeout) {
        std::shared_ptr<CGraph::GMessageParam> value;
        CStatus status;
        {
            pybind11::gil_scoped_release gsr;
            status = manager()->subTopicValue(connId, value, timeout);
        }
        return pybind11::make_tuple(status, value);
    }

    static CStatus detachTopic(const std::string& topic, CIndex connId) {
        pybind11::gil_scoped_release gsr;
        return manager()->detachConnId(topic, connId);
    }

    static CStatus dropTopic(const std::string& topic) {
        pybind11::gil_scoped_release gsr;
        return manager()->dropTopic(topic);
    }

    static CStatus clear() {
        pybind11::gil_scoped_release gsr;
        return manager()->clear();
    }

private:
    static CGraph::GMessageManager<>* manager() {
        static CGraph::USingleton<CGraph::GMessageManager<>> ugmm;
        return ugmm.get();
    }
};

#endif //CGRAPH_PYWGMESSAGE_H
