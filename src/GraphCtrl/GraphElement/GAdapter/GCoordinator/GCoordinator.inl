/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCoordinator.inl
@Time: 2023/9/8 21:12
@Desc:
***************************/

#ifndef CGRAPH_GCOORDINATOR_INL
#define CGRAPH_GCOORDINATOR_INL

#include "GCoordinator.h"

CGRAPH_NAMESPACE_BEGIN

template<CInt SIZE>
GCoordinator<SIZE>::GCoordinator() {
    element_type_ = GElementType::COORDINATOR;
    session_ = URandom<>::generateSession(CGRAPH_STR_COORDINATOR);
}


template<CInt SIZE>
CStatus GCoordinator<SIZE>::run() {
    CGRAPH_FUNCTION_BEGIN
    status = (SIZE >= 0)
             ? thread_pool_->createSecondaryThread(SIZE)
             : thread_pool_->releaseSecondaryThread((-1) * SIZE);

    CGRAPH_FUNCTION_END
}


template<CInt SIZE>
CVoid GCoordinator<SIZE>::dumpElement(std::ostream& oss) {
    dumpElementHeader(oss);
    dumpPerfInfo(oss);
    oss << "\", shape=diamond]\n";
}


template<CInt SIZE>
CStatus GCoordinator<SIZE>::checkSuitable() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(loop_ > 1,    \
    "cannot set GCoordinator loop > 1")

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GCOORDINATOR_INL