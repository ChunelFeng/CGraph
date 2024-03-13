/***************************
@Author: jianengxi
@Contact: jianengxi@gmail.com
@File: UTrieV2.h
@Time: 2024/3/9 20:35 下午
@Desc: 
***************************/

#include <stack>

#include "UTrieV2.h"
#include "UTrieNode.h"

CGRAPH_NAMESPACE_BEGIN

UTrieV2::UTrieV2() {
    head_ = CGRAPH_SAFE_MALLOC_COBJECT(UTrieNode)
}


UTrieV2::~UTrieV2() {
    clear();
    CGRAPH_DELETE_PTR(head_)
}


void UTrieV2::insert(const std::string& path) {
    auto *node = head_;
    std::string curPath{};
    for (const auto &c : path) {
        int i = (int)c;
        curPath.append(1, c);
        if(!node->children_[i]) {
            node->children_[i] = new UTrieNode(curPath);
        }
        node = node->children_[i];
    }
    node->is_end_ = true;
}


bool UTrieV2::find(const std::string& path) {
    if (head_ == nullptr) return false;
    auto *node = head_;
    for(const auto &c : path) {
        int i = (int)c;
        if(node->children_[i])
            node = node->children_[i];
        else
            return false;
    }
    return node->is_end_;
}


void UTrieV2::clear() {
    if (head_ == nullptr) return;
    std::stack<UTrieNodePtr> nodeForClear{};
    std::stack<int> path{};
    nodeForClear.push(head_);
    path.push(-1);
    while(!path.empty()) {
        auto *node = nodeForClear.top();
        bool flag = true;
        for(int i = path.top() + 1; i < MAX_TRIE_NODE_SIZE; ++i) {
            if(node->children_[i] != nullptr) {
                flag = false;
                path.pop();
                path.push(i);
                path.push(-1);
                nodeForClear.push(node->children_[i]);
                break;
            }
        }
        if(flag && node != head_) {
            nodeForClear.pop();
            path.pop();
            CGRAPH_DELETE_PTR(node);
            auto *parent = nodeForClear.top();
            parent->children_[path.top()] = nullptr;
        }
        else if(flag && node == head_) {
            path.pop();
        }
    }
}


void UTrieV2::eraser(const std::string& path) {
    if (head_ == nullptr || path.empty()) return;
    auto *node = head_;
    for(const auto &c : path) {
        int i = (int)c;
        if(node->children_[i]) 
            node = node->children_[i];
        else 
            return;
    }
    if(node->is_end_ && node->path_ == path) 
        node->is_end_ = false;
}

CGRAPH_NAMESPACE_END
