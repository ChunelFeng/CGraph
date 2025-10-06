/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UReflectionDefine.h
@Time: 2025/10/4 10:35
@Desc: 
***************************/

#ifndef CGRAPH_UREFLECTIONDEFINE_H
#define CGRAPH_UREFLECTIONDEFINE_H

#include <type_traits>
#include <utility>
#include <string_view>
#include <tuple>
#include <typeinfo>

#include <string>
#include <vector>

#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN
CGRAPH_INTERNAL_NAMESPACE_BEGIN

using UReflIdType = CUChar;

template<typename ClassType, typename FieldType>
struct UFieldTraitInfo {
    constexpr UFieldTraitInfo(FieldType ClassType::* offset,
                               const std::string_view &name, internal::UReflIdType id)
            : offset_(offset), name_(name), id_(id) {};

    FieldType ClassType::* offset_{nullptr};
    std::string_view name_{};
    internal::UReflIdType id_ {0};
};

#define UREFL_DECLARE_FIELD(_STRUCT, _FIELD, _FID)             \
    UFieldTraitInfo{ &_STRUCT::_FIELD, #_FIELD, _FID }         \

#define UREFL_CREATE_STRUCT_TRAIT_INFO(_STRUCT, ...)           \
template <>                                                    \
inline constexpr decltype(auto) createTrait<_STRUCT>() {       \
    return std::make_tuple(__VA_ARGS__); }                     \

CGRAPH_INTERNAL_NAMESPACE_END
CGRAPH_NAMESPACE_END

#endif //CGRAPH_UREFLECTIONDEFINE_H
