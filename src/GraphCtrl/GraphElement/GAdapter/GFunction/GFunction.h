/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GFunction.h
@Time: 2022/1/22 11:45 下午
@Desc: 
***************************/

#ifndef CGRAPH_GFUNCTION_H
#define CGRAPH_GFUNCTION_H

CGRAPH_NAMESPACE_BEGIN

class GFunction : public GAdapter {
public:
    /**
     * 设置执行函数具体内容
     * @param type
     * @param func
     * @return
     */
    GFunction* setFunction(const CFunctionType& type,
                           CGRAPH_CSTATUS_CONST_FUNCTION_REF func) {
        CGRAPH_ASSERT_INIT_RETURN_NULL(false)
        CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(func)

        switch (type) {
            case CFunctionType::INIT: init_function_ = func; break;
            case CFunctionType::RUN: run_function_ = func; break;
            case CFunctionType::DESTROY: destroy_function_ = func; break;
            default: return nullptr;     // 不可能出现的情况
        }

        return this;
    }

protected:
    CStatus init() override {
        return init_function_ ? init_function_() : CStatus();
    }

    CStatus run() override {
        return run_function_ ? run_function_() : CStatus();
    }

    CStatus destroy() override {
        return destroy_function_ ? destroy_function_() : CStatus();
    }

private:
    CGRAPH_CSTATUS_FUNCTION init_function_ = nullptr;
    CGRAPH_CSTATUS_FUNCTION run_function_ = nullptr;
    CGRAPH_CSTATUS_FUNCTION destroy_function_ = nullptr;

    friend class GPipeline;
};

using GFunctionPtr = GFunction *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GFUNCTION_H
