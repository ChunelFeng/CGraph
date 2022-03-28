/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNode.inl
@Time: 2021/6/1 10:13 下午
@Desc:
***************************/

#ifndef CGRAPH_GNODE_INL
#define CGRAPH_GNODE_INL

CGRAPH_NAMESPACE_BEGIN

template<typename Func, typename... Args>
CStatus GNode::doDetach(const Func&& func, Args&&... args) {
    CGRAPH_FUNCTION_BEGIN
    UThreadPoolPtr tp = UThreadPoolSingleton::get(false);
    CGRAPH_ASSERT_NOT_NULL(tp)

    tp->commit(std::bind(func, std::forward<Args>(args)...));
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GNODE_INL