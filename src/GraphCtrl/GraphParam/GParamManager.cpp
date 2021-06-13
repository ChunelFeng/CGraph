/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParamManager.cpp
@Time: 2021/6/11 6:59 下午
@Desc: 
***************************/

#include "GParamManager.h"

CSTATUS GParamManager::init() {
    CGRAPH_FUNCTION_BEGIN
    params_map_.clear();
    CGRAPH_FUNCTION_END
}


CSTATUS GParamManager::run() {
    CGRAPH_NO_SUPPORT
}


CSTATUS GParamManager::deinit() {
    CGRAPH_FUNCTION_BEGIN
    params_map_.clear();
    CGRAPH_FUNCTION_END
}
