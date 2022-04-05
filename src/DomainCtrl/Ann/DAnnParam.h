/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: DAnnParam.h
@Time: 2022/4/4 21:18
@Desc: 
***************************/

#ifndef CGRAPH_DANNPARAM_H
#define CGRAPH_DANNPARAM_H

#include <string>

#include "DAnnObject.h"
#include "DAnnDefine.h"

CGRAPH_NAMESPACE_BEGIN

struct DAnnParam : public DAnnObject, public GParam {
    CSize dim_ = 0;                                             // 维度信息
    CSize cur_vec_size_ = 0;                                    // 当前向量个数
    CSize max_vec_size_ = 0;                                    // 最大向量个数
    CBool normalize_ = false;                                   // 是否归一化处理
    std::string ann_model_path_;                                // 模型路径
    std::string train_file_path_;                               // 读取待训练数据文件的位置
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_DANNPARAM_H
