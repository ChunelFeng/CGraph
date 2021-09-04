/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPoolSingleton.cpp
@Time: 2021/8/28 11:33 下午
@Desc: 
***************************/

#include "UThreadPoolSingleton.h"

USingleton<UThreadPool> UThreadPoolSingleton::pool_;

UThreadPoolPtr UThreadPoolSingleton::get() {
    UThreadPoolPtr ptr = UThreadPoolSingleton::pool_.get();
    return ptr;
}

