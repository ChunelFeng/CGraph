/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UTrieNode.h
@Time: 2021/9/19 5:13 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTRIENODE_H
#define CGRAPH_UTRIENODE_H

#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN

const static unsigned int MAX_TRIE_NODE_SIZE = 256;

class UTrieNode : public UtilsObject {
public:
    explicit UTrieNode(const std::string& path = CGRAPH_EMPTY) {
        path_ = path;
        for (auto& child : children_) {
            child = nullptr;
        }
    }

    ~UTrieNode() override {
        for (auto& child : children_) {
            CGRAPH_DELETE_PTR(child)
        }
    }


private:
    bool is_end_ { false };
    std::string path_;
    UTrieNode* children_[MAX_TRIE_NODE_SIZE]{};

    friend class UTrie;
};

using UTrieNodePtr = UTrieNode *;
using UTrieNodeRef = UTrieNode &;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTRIENODE_H
