/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPoolSingleton.cpp
@Time: 2021/8/28 11:33 下午
@Desc: 
***************************/

#include "UThreadPoolSingleton.h"

CGRAPH_NAMESPACE_BEGIN

USingleton<UThreadPool> UThreadPoolSingleton::s_pool_;

UThreadPoolPtr UThreadPoolSingleton::get(bool autoInit) {
    UThreadPoolPtr ptr = UThreadPoolSingleton::s_pool_.get();
    autoInit ? ptr->init() : CStatus();
    return ptr;
}

CGRAPH_NAMESPACE_END
