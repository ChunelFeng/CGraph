/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGNode.h
@Time: 2025/1/30 21:41
@Desc: 
***************************/

#ifndef CGRAPH_PYWGNODE_H
#define CGRAPH_PYWGNODE_H

#include <pybind11/pybind11.h>

#include "CGraph.h"

namespace py = pybind11;

class PywGNode : public CGraph::GNode {
public:
    explicit PywGNode(const std::string& name,
                      int loop = CGraph::CGRAPH_DEFAULT_LOOP_TIMES) {
        setName(name);
        setLoop(loop);
    }

    explicit PywGNode(const CGraph::GElementPtrSet& depends = CGraph::GElementPtrSet{},
                      const std::string& name = CGraph::CGRAPH_EMPTY,
                      int loop = CGraph::CGRAPH_DEFAULT_LOOP_TIMES) {
        addDependGElements(depends);
        setLoop(loop);
        setName(name);
    }

protected:
    CStatus init() override {
        PYBIND11_OVERLOAD(CStatus, GNode, init);
    }

    CStatus run() override {
        PYBIND11_OVERLOAD_PURE(CStatus, GNode, run);
    }

    CStatus destroy() override {
        PYBIND11_OVERLOAD(CStatus, GNode, destroy);
    }

    CBool isHold() override {
        PYBIND11_OVERLOAD(CBool, GNode, isHold);
    }

    CBool isMatch() override {
        PYBIND11_OVERLOAD(CBool, GNode, isMatch);
    }
};

#endif //CGRAPH_PYWGNODE_H
