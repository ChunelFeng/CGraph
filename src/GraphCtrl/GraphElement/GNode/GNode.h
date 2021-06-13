/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNode.h
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

#ifndef CGRAPH_GNODE_H
#define CGRAPH_GNODE_H

#include "../GElement.h"
#include "../../GraphParam/GParamManager.h"

class GNode : public GElement {

public:
    /**
     * 获取参数信息，如果未找到，则返回nullptr
     * @tparam T
     * @param key
     * @return
     */
    template<typename T>
    T* getGParam(const std::string& key);

    /**
     * 创建param信息，如果过了，则直接返回ok
     * @tparam T
     * @param key
     * @return
     */
    template<typename T>
    CSTATUS createGParam(const std::string& key);

protected:
    explicit GNode();
    ~GNode() override;

    GNode(const GNode& node);
    GNode& operator=(const GNode& node);

    CSTATUS process(bool isMock) override;

    CSTATUS beforeRun() override;
    CSTATUS afterRun() override;

    CSTATUS setParamManager(const GParamManagerPtr manager);

    friend class GPipeline;

private:
    GParamManagerPtr param_manager_;    // 整体流程的参数管理类，所有pipeline中的所有节点共享
};

using GNodePtr = GNode *;
using GNodePtrArr = std::vector<GNodePtr>;
using GNodePtrSet = std::set<GNodePtr>;

#include "GNode.inl"

#endif //CGRAPH_GNODE_H
