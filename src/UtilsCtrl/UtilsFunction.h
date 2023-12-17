/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UtilsFunction.h
@Time: 2022/1/26 11:27 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTILSFUNCTION_H
#define CGRAPH_UTILSFUNCTION_H

#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <cstdarg>
#include <algorithm>
#include <thread>
#include <chrono>

#include "../CBasic/CBasicInclude.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * 获取当前的ms信息
 * @return
 */
inline CMSec CGRAPH_GET_CURRENT_MS() {
    // 获取当前的时间戳信息
    return (CMSec)std::chrono::time_point_cast<std::chrono::milliseconds>    \
        (std::chrono::steady_clock::now()).time_since_epoch().count();
}


/**
 * 获取当前的ms信息(包含小数)
 * @return
 */
inline CFMSec CGRAPH_GET_CURRENT_ACCURATE_MS() {
    // 获取当前的时间戳信息
    return (CFMSec)std::chrono::time_point_cast<std::chrono::microseconds>    \
                (std::chrono::steady_clock::now()).time_since_epoch().count() / (CFMSec)1000.0;
}


/**
 * 通用容器累加信息
 * @tparam T (例：std::vector<int>)
 * @param container
 * @return
 */
template<typename T>
typename T::value_type CGRAPH_CONTAINER_SUM(const T& container) {
    typename T::value_type result = 0;
    for (const auto& val : container) {
        result += val;
    }
    return result;
}


/**
 * 通用容器累乘信息
 * @tparam T (例：std::vector<int>)
 * @param container
 * @return
 */
template<typename T>
typename T::value_type CGRAPH_CONTAINER_MULTIPLY(const T& container) {
    typename T::value_type result = 1;
    for (const auto& val : container) {
        result *= val;
    }
    return result;
}


/**
 * 获取max值
 * @tparam T
 * @param value
 * @return
 */
template <typename T>
T CGRAPH_MAX(T val) {
    return val;
}

template <typename T, typename... Args>
T CGRAPH_MAX(T val, Args... args) {
    return std::max(val, CGRAPH_MAX(args...));
}


/**
 * 累加
 * @tparam T
 * @param t
 * @return
 */
template<typename T>
T CGRAPH_SUM(T t) {
    return t;
}

template<typename T, typename... Args>
T CGRAPH_SUM(T val, Args... args) {
    return val + CGRAPH_SUM(args...);
}


/**
 * 模拟的yield操作，兼容了 sleep 和 yield 两种情况
 * 考虑到yield 在不同的os中，会有不同的实现方式，提供不同实现版本的yield方法
 * @return
 */
inline CVoid CGRAPH_YIELD() {
#ifdef _CGRAPH_SLEEP_MS_AS_YIELD_
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
#elif _CGRAPH_SLEEP_US_AS_YIELD_
    std::this_thread::sleep_for(std::chrono::microseconds(1));
#elif _CGRAPH_SLEEP_NS_AS_YIELD_
    std::this_thread::sleep_for(std::chrono::nanoseconds(1));
#else
    // 默认情况下，还是直接调用系统的yield
    std::this_thread::yield();
#endif
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTILSFUNCTION_H
