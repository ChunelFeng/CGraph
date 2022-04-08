/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: DAnnDefine.h
@Time: 2022/4/4 21:17
@Desc: 
***************************/

#ifndef CGRAPH_DANNDEFINE_H
#define CGRAPH_DANNDEFINE_H

#include "DAnnObject.h"

CGRAPH_NAMESPACE_BEGIN

enum class D_ANN_FUNC_TYPE {
    ANN_PREPARE_ERROR = 0,   // 参数处理错误
    ANN_TRAIN = 1,           // 训练
    ANN_SEARCH = 2,          // 查询
    ANN_INSERT = 3,          // 插入
    ANN_UPDATE = 4,          // 修改
    ANN_REMOVE = 5,          // 删除
    ANN_LOAD_MODEL = 6,      // 加载模型
    ANN_SAVE_MODEL = 7,      // 保存模型
    ANN_EDITION = 8,         // 其他
    ANN_MAX_SIZE = 9         // 标记大小使用，无实意
};

CGRAPH_NAMESPACE_END

using DAnnFuncType = CGraph::D_ANN_FUNC_TYPE;

#endif //CGRAPH_DANNDEFINE_H
