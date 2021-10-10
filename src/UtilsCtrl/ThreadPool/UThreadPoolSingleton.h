/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UThreadPoolSingleton.h
@Time: 2021/8/28 10:52 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTHREADPOOLSINGLETON_H
#define CGRAPH_UTHREADPOOLSINGLETON_H

#include "UThreadPool.h"
#include "../Singleton/USingleton.h"

CGRAPH_NAMESPACE_BEGIN

class UThreadPoolSingleton : public UThreadObject {
public:
    /**
     * 获取线程池指针内容
     * @return
     */
    static UThreadPoolPtr get();

private:
    UThreadPoolSingleton() = default;
    ~UThreadPoolSingleton() override = default;

    static USingleton<UThreadPool> pool_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTHREADPOOLSINGLETON_H
