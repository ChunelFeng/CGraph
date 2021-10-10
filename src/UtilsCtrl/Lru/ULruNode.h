/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: ULruNode.h
@Time: 2021/9/19 12:17 上午
@Desc: 
***************************/

#ifndef CGRAPH_ULRUNODE_H
#define CGRAPH_ULRUNODE_H

#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename K, typename V>
struct ULruNode : public UtilsObject {
    K key_;
    V value_;

    ULruNode(const K& key, const V& value) {
        this->key_ = key;
        this->value_ = value;
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_ULRUNODE_H
