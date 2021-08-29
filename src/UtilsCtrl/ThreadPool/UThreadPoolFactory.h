/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPoolFactory.h
@Time: 2021/8/28 10:52 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTHREADPOOLFACTORY_H
#define CGRAPH_UTHREADPOOLFACTORY_H

#include "UThreadPool.h"
#include "../Singleton/USingleton.h"


class UThreadPoolFactory : public UThreadObject {
public:
    /**
     * 获取线程池指针内容
     * @return
     */
    static UThreadPoolPtr get();

private:
    UThreadPoolFactory() = default;
    ~UThreadPoolFactory() = default;

    static USingleton<UThreadPool> pool_;
};


#endif //CGRAPH_UTHREADPOOLFACTORY_H
