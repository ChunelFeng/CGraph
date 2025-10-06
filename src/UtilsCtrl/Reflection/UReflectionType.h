/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UReflectionType.h
@Time: 2025/10/6 12:17
@Desc: 
***************************/

#ifndef CGRAPH_UREFLECTIONTYPE_H
#define CGRAPH_UREFLECTIONTYPE_H

#include "UReflectionDefine.h"
#include "UReflectionSession.h"

CGRAPH_NAMESPACE_BEGIN
CGRAPH_INTERNAL_NAMESPACE_BEGIN

template<typename T>
constexpr static decltype(auto) createTrait();

template<typename T>
struct is_vector_container : std::false_type {};

template<typename U, typename Alloc>
struct is_vector_container<std::vector<U, Alloc>> : std::true_type {};

template<typename T>
inline constexpr bool is_vector_container_v = is_vector_container<std::decay_t<T>>::value;

template<typename T>
inline static constexpr bool is_string_v =
        std::is_same_v<std::decay_t<T>, std::string> ||
std::is_same_v<std::decay_t<T>, std::string_view>;

template<typename T, typename Enable = void>
struct container_element_type;

// for standard containers providing value_type
template<typename T>
struct container_element_type<T, std::void_t<typename std::decay_t<T>::value_type>> {
using type = typename std::decay_t<T>::value_type;
};

template<typename T>
using container_element_t = typename container_element_type<T>::type;


enum UReflWireType : CUChar {
    kUReflDefault = 0,
    kUReflLengthDelimited = 2,
    kUReflFixed32 = 5,
};

constexpr static size_t kUReflWireTypeSize = sizeof(UReflWireType);
constexpr static size_t kUReflIdTypeSize = sizeof(UReflIdType);
constexpr static size_t kUReflSizeTTypeSize = sizeof(size_t);

template<typename ET, typename = std::enable_if_t<std::is_enum_v<ET>>>
struct UReflEnum : public CStruct {
    static constexpr size_t kEnumLen = sizeof(ET);

    static void reflGetSize(UReflSession &session, const ET &value) {
        session.addLen(kUReflIdTypeSize + kUReflWireTypeSize + kEnumLen);
    }

    static void reflRead(UReflSession &session, ET &value) {
        UReflIdType id = 0;
        session.getValueAndOffset(id);
        UReflWireType type;
        session.getValueAndOffset(type);
        session.getValueAndOffset(value);
    }

    static void reflWrite(UReflSession &session, const ET &value, UReflIdType id) {
        session.setValueAndOffset(id);
        session.setValueAndOffset(UReflWireType::kUReflFixed32);
        session.setValueAndOffset(value);
    }
};


struct UReflString : public CStruct {
    static void reflGetSize(UReflSession &session, const std::string &value) {
        session.addLen(kUReflIdTypeSize + kUReflWireTypeSize + kUReflSizeTTypeSize + value.size());
    }

    static void reflRead(UReflSession &session, std::string &value) {
        UReflIdType id = 0;
        session.getValueAndOffset(id);
        UReflWireType type;
        session.getValueAndOffset(type);

        size_t len = 0;
        session.getValueAndOffset(len);
        session.getStringAndOffset(value, len);
    }

    static void reflWrite(UReflSession &session, const std::string &value, UReflIdType id) {
        session.setValueAndOffset(id);
        session.setValueAndOffset(UReflWireType::kUReflLengthDelimited);
        const size_t &len = value.size();
        session.setValueAndOffset(len);
        session.setBufferAndOffset((CUCharPtr) (value.data()), len);
    }
};


template<typename T>
struct UReflFundamental : public CStruct  {
    static constexpr size_t kFundamentalTypeLen = sizeof(T);

    struct type {
        static void reflGetSize(UReflSession &session, const T &value) {
            session.addLen(kUReflIdTypeSize + kUReflWireTypeSize + kFundamentalTypeLen);
        }

        static void reflRead(UReflSession &session, T &value) {
            UReflIdType id = 0;
            session.getValueAndOffset(id);
            UReflWireType type;
            session.getValueAndOffset(type);
            session.getValueAndOffset(value);
        }

        static void reflWrite(UReflSession &session, const T &value, UReflIdType id) {
            session.setValueAndOffset(id);
            session.setValueAndOffset(UReflWireType::kUReflFixed32);
            session.setValueAndOffset(value);
        }
    };
};


template<typename T>
void reflGetSize(UReflSession &session, const T &value) {
    if constexpr (std::is_fundamental_v<T>) {
        UReflFundamental<T>::type::reflGetSize(session, value);
    } else if constexpr (std::is_enum_v<T>) {
        UReflEnum<T>::reflGetSize(session, value);
    } else if constexpr (is_string_v<T>) {
        UReflString::reflGetSize(session, value);
    } else if constexpr (is_vector_container_v<T>) {
        session.addLen(kUReflIdTypeSize + kUReflWireTypeSize + kUReflSizeTTypeSize);
        for (const auto &elem: value) {
            reflGetSize(session, elem);
        }
    } else if constexpr (std::is_class_v<T>) {
        static constexpr auto traits = createTrait<std::decay_t<T>>();
        std::apply([&](auto &&... info) {
            ((reflGetSize(session, value.*(info.offset_))), ...);
        }, traits);
    } else {
        throw CException(std::string("get size unsupported type: ") + typeid(T).name());
    }
}


template<typename T>
void reflWrite(UReflSession &session, const T &value, UReflIdType id = 0) {
    if constexpr (std::is_fundamental_v<T>) {
        UReflFundamental<T>::type::reflWrite(session, value, id);
    } else if constexpr (std::is_enum_v<T>) {
        UReflEnum<T>::reflWrite(session, value, id);
    } else if constexpr (is_string_v<T>) {
        UReflString::reflWrite(session, value, id);
    } else if constexpr (is_vector_container_v<T>) {
        session.setValueAndOffset(id);
        session.setValueAndOffset(UReflWireType::kUReflLengthDelimited);

        size_t cur_offset = session.getCurrentOffset();
        CUCharPtr origin_pos = session.getCurrentPos();
        size_t tag_pos = 0;
        session.setValueAndOffset(tag_pos);

        for (const auto &elem: value) {
            reflWrite(session, elem, id);
        }

        size_t total_size = (session.getCurrentPos() - origin_pos) - kUReflSizeTTypeSize;
        session.setPosValue(cur_offset, reinterpret_cast<CUCharPtr>(&total_size), kUReflSizeTTypeSize);
    } else if constexpr (std::is_class_v<T>) {
        static constexpr auto traits = createTrait<std::decay_t<T>>();
        std::apply([&](auto &&... info) {
            ((reflWrite(session, value.*(info.offset_), info.id_)), ...);
        }, traits);
    } else {
        throw CException(std::string("reflWrite unsupported type: ") + typeid(T).name());
    }
}


template<typename T>
void reflRead(UReflSession &session, T &value) {
    if constexpr (std::is_fundamental_v<T>) {
        UReflFundamental<T>::type::reflRead(session, value);
    } else if constexpr (std::is_enum_v<T>) {
        UReflEnum<T>::reflRead(session, value);
    } else if constexpr (is_string_v<T>) {
        UReflString::reflRead(session, value);
    } else if constexpr (is_vector_container_v<T>) {
        UReflIdType id = 0;
        session.getValueAndOffset(id);
        UReflWireType type;
        session.getValueAndOffset(type);

        size_t size = 0;
        session.getValueAndOffset(size);
        UReflSession subSess(session.getCurrentPos(), size);
        while (!subSess.isEmpty()) {
            auto elem = container_element_t<T>{};
            reflRead(subSess, elem);
            value.emplace_back(std::move(elem));
        }
        session.addOffset(size);
    } else if constexpr (std::is_class_v<T>) {
        static constexpr auto traits = createTrait<std::decay_t<T>>();
        std::apply([&](auto &&... info) {
            ((reflRead(session, value.*(info.offset_))), ...);
        }, traits);
    } else {
        throw CException(std::string("reflWrite unsupported type: ") + typeid(T).name());
    }
}

CGRAPH_INTERNAL_NAMESPACE_END
CGRAPH_NAMESPACE_END

#endif //CGRAPH_UREFLECTIONTYPE_H
