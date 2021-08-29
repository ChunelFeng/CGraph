/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPoolFactory.cpp
@Time: 2021/8/28 11:33 下午
@Desc: 
***************************/

#include "UThreadPoolFactory.h"

USingleton<UThreadPool> UThreadPoolFactory::pool_;

UThreadPoolPtr UThreadPoolFactory::get() {
    UThreadPoolPtr ptr = UThreadPoolFactory::pool_.get();
    return ptr;
}

