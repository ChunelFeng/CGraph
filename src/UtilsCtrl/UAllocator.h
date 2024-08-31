/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UAllocator.h
@Time: 2021/10/28 9:15 下午
@Desc: 
***************************/

#ifndef CGRAPH_UALLOCATOR_H
#define CGRAPH_UALLOCATOR_H

#include <new>
#include <mutex>
#include <memory>

#include "../CBasic/CBasicInclude.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * 仅用于生成CObject类型的类
 */
class UAllocator : public CObject {
public:
    /**
     * 生成一个 CObject 对象
     * @tparam T
     * @return T*
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<CObject, T>::value, int> = 0>
    static T* safeMallocCObject() {
        return safeMalloc<T>();
    }

    /**
     * 生成一个 CStruct 的对象
     * @tparam T
     * @return T*
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<CStruct, T>::value, int> = 0>
    static T* safeMallocCStruct() {
        return safeMalloc<T>();
    }

    /**
     * 生成带参数的普通指针
     * @tparam T
     * @tparam Args
     * @param args
     * @return T*
     */
    template<typename T, typename ...Args,
            c_enable_if_t<std::is_base_of<CObject, T>::value, int> = 0>
    static T* safeMallocTemplateCObject(Args&&... args) {
        T* result = nullptr;
        while (!result) {
            result = new(std::nothrow) T(std::forward<Args&&>(args)...);
        }
        return result;
    }

    /**
     * 生成unique智能指针信息
     * @tparam T
     * @return std::unique_ptr<T>
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<CObject, T>::value, int> = 0>
    static std::unique_ptr<T> makeUniqueCObject() {
        return c_make_unique<T>();
    }

private:
    /**
     * 生成T类型的对象
     * @tparam T
     * @return T*
     */
    template<class T>
    static T* safeMalloc() {
        T* ptr = nullptr;
        while (!ptr) {
            ptr = new(std::nothrow) T();
        }
        return ptr;
    }
};


#define CGRAPH_SAFE_MALLOC_COBJECT(Type)                         \
    UAllocator::safeMallocCObject<Type>();                       \

#define CGRAPH_MAKE_UNIQUE_COBJECT(Type)                         \
    UAllocator::makeUniqueCObject<Type>();                       \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UALLOCATOR_H
