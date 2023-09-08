/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCoordinator.h
@Time: 2023/9/8 21:12
@Desc: 
***************************/

#ifndef CGRAPH_GCOORDINATOR_H
#define CGRAPH_GCOORDINATOR_H

#include "../GAdapter.h"

CGRAPH_NAMESPACE_BEGIN

template<CInt SIZE>
class GCoordinator : public GAdapter {
protected:
    CStatus run() final {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_NOT_NULL(thread_pool_)
        status = (SIZE >= 0)
                ? thread_pool_->createSecondaryThread(SIZE)
                : thread_pool_->releaseSecondaryThread((-1) * SIZE);

        CGRAPH_FUNCTION_END
    }
};

template<CInt SIZE>
using GCoordinatorPtr = GCoordinator<SIZE> *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GCOORDINATOR_H
