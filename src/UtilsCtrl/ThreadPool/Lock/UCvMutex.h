/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UCvMutex.h
@Time: 2024/12/13 20:27
@Desc: 
***************************/

#ifndef CGRAPH_UCVMUTEX_H
#define CGRAPH_UCVMUTEX_H

#include <mutex>
#include <condition_variable>

#include "../UThreadObject.h"

CGRAPH_NAMESPACE_BEGIN

struct UCvMutex : public UThreadObject {
    std::mutex mtx_ {};
    std::condition_variable cv_ {};
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UCVMUTEX_H
