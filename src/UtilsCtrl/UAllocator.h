/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UAllocator.h
@Time: 2021/10/28 9:15 下午
@Desc: 
***************************/

#ifndef CGRAPH_UALLOCATOR_H
#define CGRAPH_UALLOCATOR_H

#ifdef _GENERATE_SESSION_
    #include <uuid/uuid.h>
#endif

#include "../CObject/CObject.h"
#include "ThreadPool/UThreadPoolDefine.h"

CGRAPH_NAMESPACE_BEGIN

static std::mutex g_session_mtx;

class UAllocator : public CObject {
public:
    template<typename T>
    static T* safeMallocCObject() {
        T* ptr = nullptr;
        while (!ptr && std::is_base_of<CObject, T>::value) {
            ptr = new(std::nothrow) T();
        }
        return ptr;
    }

    template<typename T>
    static std::unique_ptr<T> makeUniqueCObject() {
        if constexpr (std::is_base_of<CObject, T>::value) {
            return std::make_unique<T>();
        }
        return nullptr;    // 如果是非CObject类，直接返回nullptr信息
    }

    static std::string generateSession() {
        #ifdef _GENERATE_SESSION_
                CGRAPH_LOCK_GUARD lock{ g_session_mtx };
                uuid_t uuid;
                char session[36] = {0};    // 36是特定值
                uuid_generate(uuid);
                uuid_unparse(uuid, session);

                return session;
        #else
                return "";    // 非mac平台，暂时不支持自动生成session信息
        #endif
    }
};

#define CGRAPH_SAFE_MALLOC_COBJECT(TYPE)                         \
    UAllocator::safeMallocCObject<TYPE>();                       \

#define CGRAPH_MAKE_UNIQUE_COBJECT(TYPE)                         \
    UAllocator::makeUniqueCObject<TYPE>();                       \

#define CGRAPH_GENERATE_SESSION()                                \
    UAllocator::generateSession();                               \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UALLOCATOR_H
