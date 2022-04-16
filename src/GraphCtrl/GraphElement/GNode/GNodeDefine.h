/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNodeDefine.h
@Time: 2022/1/24 9:56 下午
@Desc: 
***************************/

#ifndef CGRAPH_GNODEDEFINE_H
#define CGRAPH_GNODEDEFINE_H

#include <utility>
#include <string>
#include <set>

#include "../GElement.h"

CGRAPH_NAMESPACE_BEGIN

enum class GNodeType {
    BASIC = 0,
    IO = 1,
    CPU = 2,
    GPU = 3
};

/**
 * 功能节点信息
 * 创建GCluster和GRegion的中的GNode的时候，使用到的信息
 * 创建GCluster的时候，dependence为空
 * 创建GRegion的时候，dependence不为空。若为空，则表示可直接执行
 */
struct GNodeInfo : public GraphObject {
    std::string name_;                                   // 名称
    CSize loop_ { CGRAPH_DEFAULT_LOOP_TIMES } ;          // 循环次数
    GElementPtrSet dependence_;                          // 依赖节点

    // 无依赖版本，适用于cluster创建
    explicit GNodeInfo(const std::string& name = CGRAPH_EMPTY,
                       CSize loop = CGRAPH_DEFAULT_LOOP_TIMES) {
        this->name_ = name;
        this->loop_ = loop;
    }

    // 有依赖版本，适用于region创建
    explicit GNodeInfo(const GElementPtrSet& dependence = std::initializer_list<GElementPtr>(),
                       const std::string& name = CGRAPH_EMPTY,
                       CSize loop = CGRAPH_DEFAULT_LOOP_TIMES) {
        this->name_ = name;
        this->loop_ = loop;
        this->dependence_ = dependence;
    }

private:
    CStatus run() override {
        CGRAPH_NO_SUPPORT
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GNODEDEFINE_H
