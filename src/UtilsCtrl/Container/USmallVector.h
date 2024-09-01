/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: USmallVector.h
@Time: 2024/8/4 10:20
@Desc: 
***************************/

#ifndef CGRAPH_USMALLVECTOR_H
#define CGRAPH_USMALLVECTOR_H

#include <vector>
#include <memory>
#include <cstring>

#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN

template<class T, CSize CAPACITY=8>
class USmallVector : public UtilsObject {
public:
    explicit USmallVector() {
        capacity_ = 0;
        cur_index_ = 0;
    }

    ~USmallVector() override {
        CGRAPH_DELETE_PTR_ARRAY(data_)
    }

    /**
     * 插入具体内容
     * @param val
     * @param isUnique
     * @return
     */
    CVoid insert(const T& val, CBool isUnique=true) {
        if (isUnique && hasValue(val)) {
            return;
        }

        if (cur_index_ >= capacity_) {
            // 插入超限的时候，开始扩容
            const CSize curCapacity = capacity_ + CAPACITY;
            T* curData = new T[curCapacity];
            CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(curData)

            std::move(data_, data_ + capacity_, curData);
            CGRAPH_DELETE_PTR_ARRAY(data_)
            data_ = curData;
            capacity_ = curCapacity;
        }
        data_[cur_index_++] = val;
    }

    CSize size() const {
        return cur_index_;
    }

    CBool empty() const {
        return 0 == cur_index_;
    }

    CVoid clear() {
        // 这里只清空内容，不回收内存信息
        cur_index_ = 0;
        memset(data_, 0, sizeof(T) * capacity_);
    }

    CBool remove(const T& val) {
        CBool result = false;
        for (CSize i = 0; i < cur_index_; i++) {
            if (data_[i] == val) {
                for (CSize j = i; j < cur_index_ - 1; j++) {
                    data_[j] = data_[j + 1];
                }
                cur_index_--;
                result = true;
                break;
            }
        }
        return result;
    }


    /**
     * 判断内部是否包含该值
     * @param val
     * @return
     */
    CBool hasValue(const T& val) const {
        CBool result = false;
        if (!data_) { return result; }
        for (CSize i = 0; i < cur_index_; i++) {
            if (val == data_[i]) {
                result = true;
                break;
            }
        }

        return result;
    }

    /**
     * 给vector赋值
     * @return
     */
    std::vector<T> asVector() const {
        std::vector<T> vec;
        if (!data_) {
            return vec;
        }

        for (const auto& x : *this) {
            vec.emplace_back(x);
        }
        return vec;
    }

protected:
    class UIter {
        public:
            explicit UIter(T* ptr) : ptr_(ptr) {}
            T& operator*() const { return *ptr_; }
            T* operator->() { return ptr_; }
            UIter& operator++() { ++ptr_; return *this; }
            UIter operator++(int) { UIter iter = *this; ++(*this); return iter; }
            CBool operator==(const UIter& iter) const { return ptr_ == iter.ptr_; }
            CBool operator!=(const UIter& iter) const { return ptr_ != iter.ptr_; }

        private:
            T* ptr_ { nullptr };
    };

public:
    UIter begin() const { return UIter(data_); }
    UIter end() const { return UIter(data_ + cur_index_); }
    T front() const { return data_[0]; }
    T back() const { return data_[cur_index_ - 1]; }
    T& operator[](CSize index) { return data_[index]; }
    const T& operator[](CSize index) const { return data_[index]; }

private:
    T* data_ { nullptr };          // 存放具体数据
    CSize cur_index_ { 0 };        // 当前元素数量
    CSize capacity_ { 0 };         // 数组容量
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_USMALLVECTOR_H
