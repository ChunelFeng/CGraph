/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphSkeletonDefine.h
@Time: 2021/6/5 4:49 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPIPELINEDEFINE_H
#define CGRAPH_GPIPELINEDEFINE_H

#include <utility>
#include <vector>
#include <string>
#include <set>

#include "../GraphElement/GElementInclude.h"

/**
 * 创建GCluster和GRegion的中的GNode的时候，使用到的信息
 * 创建GCluster的时候，dependence为空
 * 创建GRegion的时候，dependence不为空
 */

CGRAPH_NAMESPACE_BEGIN

const static int CGRAPH_DEFAULT_LOOP_TIMES = 1;           // 默认循环次数信息
const static int CGRAPH_DEFAULT_ELEMENT_RUN_TTL = 0;      // 线程超时时间设定，0为不设定超时信息，单位毫秒

struct GNodeInfo {
    std::string name_;
    int loop_ { CGRAPH_DEFAULT_LOOP_TIMES } ;
    GElementPtrSet dependence_;

    // 无依赖版本，适用于cluster创建
    explicit GNodeInfo(const std::string& name = "",
              int loop = CGRAPH_DEFAULT_LOOP_TIMES) {
        this->name_ = name;
        this->loop_ = loop;
    }

    // 有依赖版本，适用于region创建
    explicit GNodeInfo(const GElementPtrSet& dependence = std::initializer_list<GElementPtr>(),
              const std::string& name = "",
              int loop = CGRAPH_DEFAULT_LOOP_TIMES) {
        this->name_ = name;
        this->loop_ = loop;
        this->dependence_ = dependence;
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPIPELINEDEFINE_H
