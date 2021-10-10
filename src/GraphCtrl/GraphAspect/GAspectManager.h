/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectManager.h
@Time: 2021/10/2 10:17 下午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECTMANAGER_H
#define CGRAPH_GASPECTMANAGER_H

#include "GAspectObject.h"
#include "GAspect.h"

CGRAPH_NAMESPACE_BEGIN

class GAspectManager : public GAspectObject {
public:
    explicit GAspectManager() = default;

    ~GAspectManager() override {
        for (GAspectPtr aspect : aspect_arr_) {
            CGRAPH_DELETE_PTR(aspect);
        }

        aspect_arr_.clear();
    }

    GAspectManager& operator=(const GAspectManager& manager) {
        if (this == &manager) {
            return *this;
        }

        this->aspect_arr_ = manager.aspect_arr_;
        return (*this);
    }

    GAspectManager(const GAspectManager& manager) : GAspectObject(manager) {
        this->aspect_arr_ = manager.aspect_arr_;
    }

    /**
     * 执行切面逻辑
     */
    CSTATUS reflect(const GAspectType type, CSTATUS curStatus = STATUS_OK) {
        CGRAPH_FUNCTION_BEGIN
        if (aspect_arr_.empty()) {
            /** 如果为空，则不执行即可。属于正常情况 */
            CGRAPH_FUNCTION_END
        }

        for (GAspectPtr aspect : aspect_arr_) {
            switch (type) {
                // 仅针对Begin对应的内容，进行返回值判断
                case GAspectType::BEGIN_INIT : status = aspect->beginInit(); break;
                case GAspectType::FINISH_INIT : aspect->finishInit(curStatus); break;
                case GAspectType::BEGIN_RUN : status = aspect->beginRun(); break;
                case GAspectType::FINISH_RUN :  aspect->finishRun(curStatus); break;
                case GAspectType::BEGIN_DEINIT : status = aspect->beginDeinit(); break;
                case GAspectType::FINISH_DEINIT : aspect->finishDeinit(curStatus); break;
                default: status = STATUS_ERR;    // 超出预期范围，理论不存在的情况
            }
        }

        CGRAPH_FUNCTION_END
    }

    /**
     * 添加切面信息
     * @param aspect
     * @return
     */
    CSTATUS addAspect(GAspectPtr aspect) {
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
            aspect->setName(name);
        }

        return this;
    }

    /**
     * 获取切面个数信息
     * @return
     */
    int getSize() {
        int size = (int)aspect_arr_.size();
        return size;
    }

private:
    GAspectPtrArr aspect_arr_;

    friend class GElement;
};

using GAspectManagerPtr = GAspectManager *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GASPECTMANAGER_H
