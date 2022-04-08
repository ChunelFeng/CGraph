/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: DAnnNode.cpp
@Time: 2022/4/4 23:49
@Desc: 
***************************/

#include "DAnnNode.h"

CGRAPH_NAMESPACE_BEGIN

DAnnNode::DAnnNode() {
    setType(GNodeType::CPU);    // 计算密集型算子

    /** 初始化函数映射关系 */
    ann_func_arr_[static_cast<CUint>(DAnnFuncType::ANN_TRAIN)] = &DAnnNode::train;
    ann_func_arr_[static_cast<CUint>(DAnnFuncType::ANN_SEARCH)] = &DAnnNode::search;
    ann_func_arr_[static_cast<CUint>(DAnnFuncType::ANN_INSERT)] = &DAnnNode::insert;
    ann_func_arr_[static_cast<CUint>(DAnnFuncType::ANN_UPDATE)] = &DAnnNode::update;
    ann_func_arr_[static_cast<CUint>(DAnnFuncType::ANN_REMOVE)] = &DAnnNode::remove;
    ann_func_arr_[static_cast<CUint>(DAnnFuncType::ANN_LOAD_MODEL)] = &DAnnNode::loadModel;
    ann_func_arr_[static_cast<CUint>(DAnnFuncType::ANN_SAVE_MODEL)] = &DAnnNode::saveModel;
    ann_func_arr_[static_cast<CUint>(DAnnFuncType::ANN_EDITION)] = &DAnnNode::edition;
}


CStatus DAnnNode::run() {
    /**
     * 整体流程思路
     * 1，先准备参数，并且确定走哪个功能函数（必须实现）
     * 2，执行具体功能函数
     * 3，如有参数修改，将最终的参数同步回主流程
     */
    CGRAPH_FUNCTION_BEGIN
    const DAnnFuncType& funcType = prepareParam();
    if (unlikely(funcType <= DAnnFuncType::ANN_PREPARE_ERROR
        || funcType >= DAnnFuncType::ANN_MAX_SIZE)) {
        CGRAPH_RETURN_ERROR_STATUS("error ann function type")
    }

    status = (this->*ann_func_arr_[static_cast<CUint>(funcType)])();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = refreshParam();
    CGRAPH_FUNCTION_END
}


CStatus DAnnNode::train() {
    CGRAPH_NO_SUPPORT
}


CStatus DAnnNode::search() {
    CGRAPH_NO_SUPPORT
}


CStatus DAnnNode::insert() {
    CGRAPH_NO_SUPPORT
}


CStatus DAnnNode::update() {
    CGRAPH_NO_SUPPORT
}


CStatus DAnnNode::remove() {
    CGRAPH_NO_SUPPORT
}


CStatus DAnnNode::loadModel() {
    CGRAPH_NO_SUPPORT
}


CStatus DAnnNode::saveModel() {
    CGRAPH_NO_SUPPORT
}


CStatus DAnnNode::edition() {
    CGRAPH_NO_SUPPORT
}


CStatus DAnnNode::refreshParam() {
    CGRAPH_EMPTY_FUNCTION
}

CGRAPH_NAMESPACE_END
