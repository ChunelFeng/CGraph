/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CStatus.h
@Time: 2021/12/17 10:32 下午
@Desc: 命名为 CSTATUS，直接对外提供的是 CStatus 类
***************************/

#ifndef CGRAPH_CSTATUS_H
#define CGRAPH_CSTATUS_H

#include <string>

#include "CBasicDefine.h"

CGRAPH_NAMESPACE_BEGIN

static const int STATUS_OK = 0;                                 /** 正常流程返回值 */
static const int STATUS_ERR = -1;                               /** 异常流程返回值 */
static const char* STATUS_ERROR_INFO_CONNECTOR = " && ";        /** 多异常信息连接符号 */

class CSTATUS {
public:
    explicit CSTATUS() = default;

    explicit CSTATUS(const std::string &errorInfo) {
        this->error_code_ = STATUS_ERR;    // 默认的error code信息
        this->error_info_ = errorInfo;
    }

    explicit CSTATUS(int errorCode, const std::string &errorInfo) {
        this->error_code_ = errorCode;
        this->error_info_ = errorInfo;
    }

    CSTATUS(const CSTATUS &status) {
        this->error_code_ = status.error_code_;
        this->error_info_ = status.error_info_;
    }

    CSTATUS(const CSTATUS &&status) noexcept {
        this->error_code_ = status.error_code_;
        this->error_info_ = status.error_info_;
    }

    CSTATUS& operator=(const CSTATUS& status) = default;

    CSTATUS& operator+=(const CSTATUS& cur) {
        if (this->isOK() && cur.isOK()) {
            return (*this);
        }

        error_info_ = this->isOK()
                ? cur.error_info_
                : (cur.isOK()
                    ? error_info_
                    : (error_info_ + STATUS_ERROR_INFO_CONNECTOR + cur.error_info_));
        error_code_ = STATUS_ERR;

        return (*this);
    }

    void setStatus(const std::string& info) {
        error_code_ = STATUS_ERR;
        error_info_ = info;
    }

    void setStatus(int code, const std::string& info) {
        error_code_ = code;
        error_info_ = info;
    }

    [[nodiscard]] int getCode() const {
        return this->error_code_;
    }

    [[nodiscard]] const std::string& getInfo() const {
        return this->error_info_;
    }

    /**
     * 判断当前状态是否可行
     * @return
     */
    [[nodiscard]] bool isOK() const {
        return STATUS_OK == error_code_;
    }

    /**
     * 判断当前状态是否可行
     * @return
     */
    [[nodiscard]] bool isErr() const {
        return error_code_ < STATUS_OK;    // 约定异常信息，均为负值
    }

private:
    int error_code_ { STATUS_OK };                   // 错误码信息
    std::string error_info_;                         // 错误信息描述
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CSTATUS_H
