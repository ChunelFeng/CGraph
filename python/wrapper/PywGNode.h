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
#include "PyWrapperMacro.h"

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

    ~PywGNode() override = default;

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

    PYCGRAPH_DECLARE_GELEMENT_COMMON_FUNCTIONS(CGraph::GNode)
};

#endif //CGRAPH_PYWGNODE_H
