/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CStdWrapper.h
@Time: 2023/1/25 22:30
@Desc: 为了在C++11版本中，兼容一些高版本C++语法
***************************/

#ifndef CGRAPH_CSTDWRAPPER_H
#define CGRAPH_CSTDWRAPPER_H

#include <memory>
#include <type_traits>

CGRAPH_NAMESPACE_BEGIN

// 兼容 std::enable_if_t 的语法
template<bool B, typename T = void>
using c_enable_if_t = typename std::enable_if<B, T>::type;

// 兼容 std::make_unique 的语法
template<typename T, typename... Args>
typename std::unique_ptr<T> c_make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CSTDWRAPPER_H
