/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStorageDefine.h
@Time: 2025/10/4 14:10
@Desc: 
***************************/

#ifndef CGRAPH_GSTORAGEDEFINE_H
#define CGRAPH_GSTORAGEDEFINE_H

#include <string>
#include <vector>
#include <typeinfo>

#include "../../GraphDefine.h"
#include "../../GraphElement/GElementInclude.h"
#include "../../GraphEvent/GEventInclude.h"

CGRAPH_NAMESPACE_BEGIN

struct _GElementStorage : public CStruct {
    std::string name_ {};
    CSize loop_ { CGRAPH_DEFAULT_LOOP_TIMES };
    std::string session_ {};
    CBool visible_ { true };
    CIndex binding_index_ { CGRAPH_DEFAULT_BINDING_INDEX };
    CLevel level_ { CGRAPH_DEFAULT_ELEMENT_LEVEL };
    CMSec timeout_ { CGRAPH_DEFAULT_ELEMENT_TIMEOUT };
    GElementTimeoutStrategy timeout_strategy_ { GElementTimeoutStrategy::AS_ERROR };
    CBool is_marco_ { false };
    std::string belong_session_ {};
    GElementType element_type_ {};
    std::string clz_name_ {};        // 记录element 的真实类型信息
    std::vector<std::string> dependence_sessions_ {};
    std::vector<_GElementStorage> children_ {};

    explicit _GElementStorage() = default;

    explicit _GElementStorage(GElementCPtr element) {
        name_ = element->getName();
        loop_ = element->getLoop();
        session_ = element->getSession();
        visible_ = element->visible_;
        binding_index_ = element->getBindingIndex();
        level_ = element->level_;
        timeout_ = element->timeout_;
        timeout_strategy_ = element->timeout_strategy_;
        is_marco_ = element->is_marco_;
        belong_session_ = element->belong_ ? element->belong_->getSession() : CGRAPH_EMPTY;
        element_type_ = element->element_type_;
        clz_name_ = typeid(*element).name();
        for (const auto* dep : element->dependence_) {
            dependence_sessions_.emplace_back(dep->getSession());
        }
        for (const auto* child : element->getChildren()) {
            children_.emplace_back(child);
        }
    }

    /**
     * 根据 storage信息，恢复element的内容
     * @param element
     * @return
     */
    CStatus recover(GElementPtr element) const {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_NOT_NULL(element)
        element->setName(name_);
        element->setLoop(loop_);
        element->setBindingIndex(binding_index_);
        element->setVisible(visible_);
        element->setLevel(level_);
        element->setTimeout(timeout_, timeout_strategy_);
        element->session_ = session_;
        if (element->isGNode()) {
            element->setMacro(is_marco_);
        }

        CGRAPH_FUNCTION_END
    }

    friend class GStorage;
};

struct _GEventStorage : public CStruct {
    explicit _GEventStorage() = default;
    explicit _GEventStorage(const std::string& key, const std::string& clz) {
        key_ = key;
        event_clz_name_ = clz;
    }

    std::string key_ {};
    std::string event_clz_name_ {};
};

struct _GPipelineStorage : public CStruct {
    std::vector<_GElementStorage> element_storages_ {};       // 记录pipeline中所有 element 的信息
    std::vector<_GEventStorage> event_storages_ {};           // 记录pipeline中所有 event 的信息
    UThreadPoolConfig thread_pool_config_ {};                 // 记录线程池配置信息
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSTORAGEDEFINE_H
