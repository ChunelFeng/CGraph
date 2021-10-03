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
    void reflect(const GAspectType type, CSTATUS status = STATUS_OK) {
        if (aspect_arr_.empty()) {
            return;
        }

        for (GAspectPtr aspect : aspect_arr_) {
            switch (type) {
                case GAspectType::BEGIN_INIT : aspect->beginInit(); break;
                case GAspectType::FINISH_INIT : aspect->finishInit(status); break;
                case GAspectType::BEGIN_RUN : aspect->beginRun(); break;
                case GAspectType::FINISH_RUN : aspect->finishRun(status); break;
                case GAspectType::BEGIN_DEINIT : aspect->beginDeinit(); break;
                case GAspectType::FINISH_DEINIT : aspect->finishDeinit(status); break;
                default: ;    // 无任务执行
            }
        }
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


    void setName(const std::string& name) override {
        for (GAspectPtr aspect : aspect_arr_) {
            aspect->setName(name);
        }
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

#endif //CGRAPH_GASPECTMANAGER_H
