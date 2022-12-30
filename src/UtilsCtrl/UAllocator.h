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

#include <mutex>
#include <memory>

#include "../CBasic/CBasicInclude.h"

CGRAPH_NAMESPACE_BEGIN

static std::mutex g_session_mtx;

/**
 * 仅用于生成CObject类型的类
 */
class UAllocator : public CObject {
public:
    /**
     * 生成普通指针信息
     * @tparam T
     * @return
     */
    template<typename T,
            std::enable_if_t<std::is_base_of<CObject, T>::value, int> = 0>
    static T* safeMallocCObject() {
        T* ptr = nullptr;
        while (!ptr) {
            ptr = new(std::nothrow) T();
        }
        return ptr;
    }

    /**
     * 生成带参数的普通指针
     * @tparam T
     * @tparam Args
     * @param args
     * @return
     */
    template<typename T, typename ...Args,
            std::enable_if_t<std::is_base_of<CObject, T>::value, int> = 0>
    static T* safeMallocTemplateCObject(Args... args) {
        T* ptr = nullptr;
        while (!ptr) {
            ptr = new T(std::forward<Args>(args)...);
        }
        return ptr;
    }


    /**
     * 生成unique智能指针信息
     * @tparam T
     * @return
     */
    template<typename T,
            std::enable_if_t<std::is_base_of<CObject, T>::value, int> = 0>
    static std::unique_ptr<T> makeUniqueCObject() {
        return std::make_unique<T>();
    }


    /**
     * 生成唯一标识信息
     * @return
     */
    static std::string generateSession() {
        #ifdef _GENERATE_SESSION_
                std::lock_guard<std::mutex> lock{ g_session_mtx };
                uuid_t uuid;
                char session[36] = {0};    // 36是特定值
                uuid_generate(uuid);
                uuid_unparse(uuid, session);

                return session;
        #else
                return CGRAPH_EMPTY;    // 非mac平台，暂时不支持自动生成session信息
        #endif
    }
};


#define CGRAPH_SAFE_MALLOC_COBJECT(Type)                         \
    UAllocator::safeMallocCObject<Type>();                       \

#define CGRAPH_MAKE_UNIQUE_COBJECT(Type)                         \
    UAllocator::makeUniqueCObject<Type>();                       \

#define CGRAPH_GENERATE_SESSION                                  \
    UAllocator::generateSession();                               \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UALLOCATOR_H
