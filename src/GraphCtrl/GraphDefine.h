/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphDefine.h
@Time: 2021/4/30 9:20 下午
@Desc: 
***************************/


#ifndef CGRAPH_GRAPHDEFINE_H
#define CGRAPH_GRAPHDEFINE_H

#include <vector>
#include "GraphNode/GraphNode.h"
#include "../UtilsCtrl/UtilsInclude.h"

const static int INIT_THREAD_NUM = 4;
const static int MAX_THREAD_NUM = 16;

using GraphNodePtr = GraphNode *;
using TaskFunc = std::function<void()>;
using GNodeArr = std::vector<GraphNodePtr>;

#endif //CGRAPH_GRAPHDEFINE_H
