/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectManager.h
@Time: 2021/10/2 10:17 下午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECTMANAGER_H
#define CGRAPH_GASPECTMANAGER_H

#include "../GraphManager.h"
#include "GAspectObject.h"
#include "GAspect.h"

CGRAPH_NAMESPACE_BEGIN

class GAspectManager : public GAspectObject,
                       public GraphManager<GAspect> {
protected:
    explicit GAspectManager() = default;

    ~GAspectManager() override {
        clear();
    }

    /**
     * 执行切面逻辑
     * @param type
     * @param curStatus
     * @return
     */
    CStatus reflect(const GAspectType& type,
                    const CStatus& curStatus = CStatus()) {
        CGRAPH_FUNCTION_BEGIN

        for (GAspectPtr aspect : aspect_arr_) {
            switch (type) {
                /**
                 * 仅针对Begin对应的内容，进行返回值判断
                 * run()方法切面更容易被执行，故放在最前方判断
                 */
                case GAspectType::BEGIN_RUN : status = aspect->beginRun(); break;
                case GAspectType::FINISH_RUN : aspect->finishRun(curStatus); break;
                case GAspectType::BEGIN_INIT : status = aspect->beginInit(); break;
                case GAspectType::FINISH_INIT : aspect->finishInit(curStatus); break;
                case GAspectType::BEGIN_DESTROY : status = aspect->beginDestroy(); break;
                case GAspectType::FINISH_DESTROY : aspect->finishDestroy(curStatus); break;
                case GAspectType::ENTER_CRASHED : aspect->enterCrashed(); break;
                default: status = CErrStatus("unknown aspect type");
            }

            CGRAPH_FUNCTION_CHECK_STATUS
        }

        CGRAPH_FUNCTION_END
    }

    /**
     * 添加切面信息
     * @param aspect
     * @return
     */
    CStatus add(GAspectPtr aspect) override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_NOT_NULL(aspect)

        aspect_arr_.emplace_back(aspect);
        CGRAPH_FUNCTION_END
    }

    /**
     * 设置name信息
     * @param name
     * @return
     */
    GAspectManager* setName(const std::string& name) override {
        for (GAspectPtr aspect : aspect_arr_) {
            CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(aspect)
            aspect->setName(name);
        }

        return this;
    }

    /**
     * 获取切面个数信息
     * @return
     */
    CSize getSize() const override {
        auto size = aspect_arr_.size();
        return size;
    }

    /**
     * 清空切面信息
     * @return
     */
    CStatus clear() final {
        CGRAPH_FUNCTION_BEGIN
        for (GAspectPtr aspect : aspect_arr_) {
            CGRAPH_DELETE_PTR(aspect)
        }

        aspect_arr_.clear();
        CGRAPH_FUNCTION_END
    }

    /**
     * 弹出去最后一个
     * @return
     */
    CStatus popLast() {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(0 == getSize(), "no aspect to pop")

        auto* last = aspect_arr_.back();
        CGRAPH_DELETE_PTR(last);
        aspect_arr_.pop_back();    // 弹出最后一个
        CGRAPH_FUNCTION_END
    }

    CGRAPH_NO_ALLOWED_COPY(GAspectManager)

private:
    GAspectPtrArr aspect_arr_;

    friend class GElement;
    friend class UAllocator;
};

using GAspectManagerPtr = GAspectManager *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GASPECTMANAGER_H
