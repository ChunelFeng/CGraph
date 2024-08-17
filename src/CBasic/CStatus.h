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
CGRAPH_INTERNAL_NAMESPACE_BEGIN

/**
 * 说明：
 * 返回值为0，表示正常逻辑
 * 返回值为负整数，表示error逻辑，程序终止执行
 * 自定义返回值，请务必遵守以上约定
 */
static const int STATUS_OK = 0;                                 /** 正常流程返回值 */
static const int STATUS_ERR = -1;                               /** 异常流程返回值 */
static const int STATUS_CRASH = -996;                           /** 异常流程返回值 */

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
        if (status.error_code_ == error_code_) {
            return;
        }

        this->error_code_ = status.error_code_;
        this->error_info_ = status.error_info_;
    }

    CSTATUS(const CSTATUS &&status) noexcept {
        if (status.error_code_ == error_code_) {
            return;
        }

        this->error_code_ = status.error_code_;
        this->error_info_ = status.error_info_;
    }

    CSTATUS& operator=(const CSTATUS& status) {
        if (this->error_code_ != status.error_code_) {
            // 如果status是正常的话，则所有数据保持不变
            this->error_code_ = status.error_code_;
            this->error_info_ = status.error_info_;
        }
        return (*this);
    }

    CSTATUS& operator+=(const CSTATUS& cur) {
        /**
         * 如果当前状态已经异常，则不做改动
         * 如果当前状态正常，并且传入的状态是异常的话，则返回异常
         */
        if (!this->isErr() && cur.isErr()) {
            this->error_code_ = cur.error_code_;
            this->error_info_ = cur.error_info_;
        }

        return (*this);
    }

    /**
     * 恢复状态信息
     */
    void reset() {
        if (this->error_code_ != STATUS_OK) {
            this->error_code_ = STATUS_OK;
            this->error_info_.clear();
        }
    }

    /**
     * 获取异常值信息
     * @return
     */
    int getCode() const {
        return this->error_code_;
    }

    /**
     * 获取异常信息
     * @return
     */
    const std::string& getInfo() const {
        return this->error_info_;
    }

    /**
     * 判断当前状态是否可行
     * @return
     */
    bool isOK() const {
        return STATUS_OK == error_code_;
    }

    /**
     * 判断当前状态是否可行
     * @return
     */
    bool isErr() const {
        return error_code_ < STATUS_OK;    // 约定异常信息，均为负值
    }

    /**
     * 判断当前状态是否是崩溃了
     * @return
     */
    bool isCrash() const {
        return STATUS_CRASH == error_code_;
    }

    /**
     * 设置异常信息
     * @param code
     * @param info
     * @return
     */
    CSTATUS* setInfo(int code, const std::string& info) {
        error_code_ = code;
        error_info_ = (STATUS_OK == error_code_) ? CGRAPH_EMPTY : info;
        return this;
    }

    /**
     * 设置异常信息
     * @param info
     * @return
     */
    CSTATUS* setErrorInfo(const std::string& info) {
        error_code_ = STATUS_ERR;
        error_info_ = info;
        return this;
    }

private:
    int error_code_ = STATUS_OK;                     // 错误码信息
    std::string error_info_;                         // 错误信息描述
};

CGRAPH_INTERNAL_NAMESPACE_END
CGRAPH_NAMESPACE_END

#endif //CGRAPH_CSTATUS_H
