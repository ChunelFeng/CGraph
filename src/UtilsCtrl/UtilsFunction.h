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
#include <ctime>
#include <cstdarg>
#include <algorithm>

CGRAPH_NAMESPACE_BEGIN

/**
 * 定制化输出
 * @param cmd
 * @param ...
 * 注：内部包含全局锁，不建议正式上线的时候使用
 */
static std::mutex g_echo_mtx;
inline CVoid CGRAPH_ECHO(const char *cmd, ...) {
#ifdef _CGRAPH_SILENCE_
    return;
#endif

    std::lock_guard<std::mutex> lock{ g_echo_mtx };
#ifndef _WIN32
    // 非windows系统，打印到毫秒
    auto now = std::chrono::system_clock::now();
    // 通过不同精度获取相差的毫秒数
    uint64_t disMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
                     - std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;
    time_t tt = std::chrono::system_clock::to_time_t(now);
    auto localTime = localtime(&tt);
    char strTime[32] = { 0 };
    sprintf(strTime, "[%04d-%02d-%02d %02d:%02d:%02d.%03d]", localTime->tm_year + 1900,
            localTime->tm_mon + 1, localTime->tm_mday, localTime->tm_hour,
            localTime->tm_min, localTime->tm_sec, (int)disMs);
    std::cout << "[CGraph] " << strTime << " ";
#else
    // windows系统，打印到秒
    time_t curTime;
    time(&curTime);
    std::string ct = ctime(&curTime);
    std::cout << "[CGraph] ["
              << ct.assign(ct.begin(), ct.end()-1)    // 去掉时间的最后一位\n信息
              << "] ";
#endif

    va_list args;
    va_start(args, cmd);
    vprintf(cmd, args);
    va_end(args);
    std::cout << "\n";
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

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTILSFUNCTION_H
