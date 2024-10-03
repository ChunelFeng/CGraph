/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectManager.cpp
@Time: 2023/12/29 19:45
@Desc: 
***************************/

#include "GAspectManager.h"

CGRAPH_NAMESPACE_BEGIN

GAspectManager::GAspectManager() {
    aspect_arr_ = {};
}


GAspectManager::~GAspectManager() {
    clear();
}


CStatus GAspectManager::reflect(const internal::GAspectType &type,
                                const CStatus &curStatus) {
    CGRAPH_FUNCTION_BEGIN

    for (GAspectPtr aspect : aspect_arr_) {
        switch (type) {
            /**
             * 仅针对Begin对应的内容，进行返回值判断
             * run()方法切面更容易被执行，故放在最前方判断
             */
            case internal::GAspectType::BEGIN_RUN :
                status = aspect->beginRun();
                break;
            case internal::GAspectType::FINISH_RUN :
                aspect->finishRun(curStatus);
                break;
            case internal::GAspectType::BEGIN_INIT :
                status = aspect->beginInit();
                break;
            case internal::GAspectType::FINISH_INIT :
                aspect->finishInit(curStatus);
                break;
            case internal::GAspectType::BEGIN_DESTROY :
                status = aspect->beginDestroy();
                break;
            case internal::GAspectType::FINISH_DESTROY :
                aspect->finishDestroy(curStatus);
                break;
            case internal::GAspectType::ENTER_CRASHED :
                aspect->enterCrashed();
                break;
            default:
                status = CStatus("unknown aspect type");
        }

        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CStatus GAspectManager::add(GAspectPtr aspect) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(aspect)
    aspect_arr_.emplace_back(aspect);
    CGRAPH_FUNCTION_END
}


CSize GAspectManager::getSize() const {
    auto size = aspect_arr_.size();
    return size;
}


CStatus GAspectManager::clear() {
    CGRAPH_FUNCTION_BEGIN
    for (GAspectPtr aspect: aspect_arr_) {
        CGRAPH_DELETE_PTR(aspect)
    }

    aspect_arr_.clear();
    CGRAPH_FUNCTION_END
}


CStatus GAspectManager::popLast() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(0 == getSize(), "no aspect to pop")

    auto *last = aspect_arr_.back();
    CGRAPH_DELETE_PTR(last);
    aspect_arr_.pop_back();    // 弹出最后一个
    CGRAPH_FUNCTION_END
}


CVoidPtr GAspectManager::setGParamManager(GParamManagerPtr pm) {
    for (auto *cur: aspect_arr_) {
        cur->setGParamManager(pm);
    }
    return this;
}


CVoidPtr GAspectManager::setGEventManager(GEventManagerPtr em) {
    for (auto *cur : aspect_arr_) {
        cur->setGEventManager(em);
    }
    return this;
}

CGRAPH_NAMESPACE_END
