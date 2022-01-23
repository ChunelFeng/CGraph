/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GFunction.h
@Time: 2022/1/22 11:45 下午
@Desc: 
***************************/

#ifndef CGRAPH_GFUNCTION_H
#define CGRAPH_GFUNCTION_H

#include "GFunctionDefine.h"

CGRAPH_NAMESPACE_BEGIN

class GFunction : public GAdapter {
public:
    /**
     * 设置执行函数具体内容
     * @param type
     * @param func
     * @return
     */
    GFunction* setFunction(const GFunctionType& type, const CGRAPH_CSTATUS_FUNCTION& func) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT_RETURN_NULL(false)
        CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(func)

        switch (type) {
            case GFunctionType::RUN: run_function_ = func; break;
            case GFunctionType::INIT: init_function_ = func; break;
            case GFunctionType::DEINIT: deinit_function_ = func; break;
            default: return nullptr;     // 不可能出现的情况
        }

        return this;
    }

protected:
    CStatus init() final {
        CGRAPH_FUNCTION_BEGIN
        if (init_function_) {
            status = init_function_();
        }

        CGRAPH_FUNCTION_END
    }

    CStatus run() final {
        CGRAPH_FUNCTION_BEGIN
        if (run_function_) {
            status = run_function_();
        }

        CGRAPH_FUNCTION_END
    }

    CStatus deinit() final {
        CGRAPH_FUNCTION_BEGIN
        if (deinit_function_) {
            status = deinit_function_();
        }

        CGRAPH_FUNCTION_END
    }

private:
    CGRAPH_CSTATUS_FUNCTION init_function_ = nullptr;
    CGRAPH_CSTATUS_FUNCTION run_function_ = nullptr;
    CGRAPH_CSTATUS_FUNCTION deinit_function_ = nullptr;

    friend class GPipeline;
};

using GFunctionPtr = GFunction *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GFUNCTION_H
