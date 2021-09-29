/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspect.inl
@Time: 2021/9/27 11:28 下午
@Desc:
***************************/


template <typename T>
CSTATUS GAspect<T>::run()  {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_NOT_NULL(GAspectObject<T>::element_obj_);
    begin();
    status = dynamic_cast<GElementPtr>(GAspectObject<T>::element_obj_)->run();
    finish();
    CGRAPH_FUNCTION_CHECK_STATUS

    CGRAPH_FUNCTION_END
}