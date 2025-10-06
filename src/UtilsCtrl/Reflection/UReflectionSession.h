/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UReflectionSession.h
@Time: 2025/10/6 12:17
@Desc: 
***************************/

#ifndef CGRAPH_UREFLECTIONSESSION_H
#define CGRAPH_UREFLECTIONSESSION_H

#include "UReflectionDefine.h"

CGRAPH_NAMESPACE_BEGIN
CGRAPH_INTERNAL_NAMESPACE_BEGIN

class UReflSession : public UtilsObject {
public:
    explicit UReflSession(CUCharPtr data, size_t len)
            : data_(data), len_(len) {}

    explicit UReflSession() = default;

    template<class T>
    inline void getValueAndOffset(T &value) {
        constexpr size_t len = sizeof(T);
        memcpy(&value, data_ + offset_, len);
        offset_ += len;
    }

    inline void setBufferAndOffset(CUCharPtr buffer, size_t len) {
        memcpy(data_ + offset_, buffer, len);
        offset_ += len;
    }

    inline void getStringAndOffset(std::string &value, size_t len) {
        value.assign((CCharPtr)(data_ + offset_), len);
        offset_ += len;
    }

    template<class T>
    inline void setValueAndOffset(const T &value) {
        constexpr size_t len = sizeof(T);
        memcpy(data_ + offset_, &value, len);
        offset_ += len;
    }

    inline void addLen(size_t len) {
        len_ += len;
    }

    inline void addOffset(size_t len) {
        offset_ += len;
    }

    inline void setPosValue(size_t offset, CUCharPtr value, size_t len) const {
        memcpy(data_ + offset, value, len);
    }

    [[nodiscard]] inline CUCharPtr getCurrentPos() const {
        return data_ + offset_;
    }

    [[nodiscard]] inline size_t getCurrentOffset() const {
        return offset_;
    }

    [[nodiscard]] inline bool isEmpty() const {
        return len_ <= offset_;
    }

    [[nodiscard]] inline size_t getLen() const {
        return len_;
    }

private:
    size_t len_{0};                         // 整体长度
    size_t offset_{0};                      // 当前偏移量
    CUCharPtr data_{nullptr};               // 真实数据
};

CGRAPH_INTERNAL_NAMESPACE_END
CGRAPH_NAMESPACE_END

#endif //CGRAPH_UREFLECTIONSESSION_H
