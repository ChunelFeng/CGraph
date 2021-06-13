/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNode.inl
@Time: 2021/6/1 10:13 下午
@Desc:
***************************/

#ifndef CGRAPH_GNODE_INL
#define CGRAPH_GNODE_INL

template<typename T>
CSTATUS GNode::createGParam(const std::string& key) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(this->param_manager_)

    status = this->param_manager_->create<T>(key);
    CGRAPH_FUNCTION_END
}


template<typename T>
T* GNode::getGParam(const std::string& key) {
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(this->param_manager_)

    T* ptr = dynamic_cast<T *>(this->param_manager_->get(key));
    return ptr;
}

#endif // CGRAPH_GNODE_INL