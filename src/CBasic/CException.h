/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CException.h
@Time: 2022/4/15 20:51
@Desc: 异常处理类
***************************/

#ifndef CGRAPH_CEXCEPTION_H
#define CGRAPH_CEXCEPTION_H

#include <string>
#include <exception>

#include "CStrDefine.h"

CGRAPH_NAMESPACE_BEGIN

class CEXCEPTION : public std::exception {
public:
    explicit CEXCEPTION(const std::string& info = CGRAPH_EMPTY) {
        info_ = info.empty() ? CGRAPH_BASIC_EXCEPTION : info;
    }

    /**
     * 获取异常信息
     * @return
     */
    [[nodiscard]] const char* what() const noexcept override {
        return info_.c_str();
    }

private:
    std::string info_;            // 异常状态信息
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CEXCEPTION_H
