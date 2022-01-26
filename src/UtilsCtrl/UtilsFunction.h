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

CGRAPH_NAMESPACE_BEGIN

static std::mutex g_echo_mtx;

/* CGRAPH 个性化输出 */
inline void CGRAPH_ECHO(const char *cmd, ...) {
#ifdef _CGRAPH_SILENCE_
    return;
#endif

    std::lock_guard<std::mutex> lock{ g_echo_mtx };
#ifndef _WIN32
    // 非windows系统，打印到毫秒
    auto now = std::chrono::system_clock::now();
    //通过不同精度获取相差的毫秒数
    uint64_t disMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
                     - std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;
    time_t tt = std::chrono::system_clock::to_time_t(now);
    auto time_tm = localtime(&tt);
    char strTime[32] = { 0 };
    sprintf(strTime, " [%d-%02d-%02d %02d:%02d:%02d.%03d]", time_tm->tm_year + 1900,
            time_tm->tm_mon + 1, time_tm->tm_mday, time_tm->tm_hour,
            time_tm->tm_min, time_tm->tm_sec, (int)disMs);
    std::cout << "[CGraph]" << strTime << " ";
#else
    // windows系统，打印到秒
    time_t cur_time = time(nullptr);
        std::string ct = ctime(&cur_time);
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
typename T::value_type CGRAPH_SUM(const T& container)
{
    typename T::value_type result = 0;
    for (const auto& val : container) {
        result += val;
    }
    return result;
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTILSFUNCTION_H
