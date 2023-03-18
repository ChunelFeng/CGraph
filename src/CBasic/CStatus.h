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
#include "CStrDefine.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * 说明：
 * 返回值为0，表示正常逻辑
 * 返回值为正整数，表示warning逻辑，程序仍会继续执行
 * 返回值为负整数，表示error逻辑，程序终止执行
 * 自定义返回值，请务必遵守以上约定
 */
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
     * 恢复数据
     */
    void reset() {
        error_code_ = STATUS_OK;
        error_info_ = CGRAPH_EMPTY;
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

    /**
     * 判断当前状态是否有异常
     * @return
     */
    [[nodiscard]] bool isNotErr() const {
        return error_code_ >= STATUS_OK;
    }

    /**
     * 判断当前状态，不是ok的（包含error 和 warning）
     * @return
     */
    [[nodiscard]] bool isNotOK() const {
        return error_code_ != STATUS_OK;
    }

private:
    int error_code_ { STATUS_OK };                   // 错误码信息
    std::string error_info_;                         // 错误信息描述
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CSTATUS_H
