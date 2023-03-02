/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: USerialUniqueArray.h
@Time: 2023/2/27 23:35
@Desc: 
***************************/

#ifndef CGRAPH_USERIALUNIQUEARRAY_H
#define CGRAPH_USERIALUNIQUEARRAY_H

#include <vector>
#include <set>

#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN

template<class T>
class USerialUniqueArray : public UtilsObject {
public:
    /**
     * 不重复的插入顺序容器中
     * @param val
     * @return
     */
    CVoid uniqueAdd(const T& val) {
        // 如果没有，就插入array中
        if (inner_set_.find(val) == inner_set_.end()) {
            inner_set_.insert(val);
            inner_array_.emplace_back(val);
        }
    }

    /**
     * 获取不重复的有序array信息
     * @param arr
     * @return
     */
    CVoid getUniqueArray(std::vector<T>& arr) {
        for (const auto& iter : inner_array_) {
            arr.push_back(iter);
        }
    }

    /**
     * 清空所有内部信息
     * @return
     */
    CVoid clear() {
        inner_set_.clear();
        inner_array_.clear();
    }

private:
    std::set<T> inner_set_;               // 内部set，留比对使用
    std::vector<T> inner_array_;          // 内部array，最终提供的排序不重复内容
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_USERIALUNIQUEARRAY_H
