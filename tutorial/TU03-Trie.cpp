/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T03-Trie.cpp
@Time: 2021/9/19 5:28 下午
@Desc: 本例主要演示，Trie的使用方法
***************************/

#include "CGraph.h"

using namespace CGraph;

void tutorial_trie() {
    UTrie trie;
    trie.insert("hello");
    trie.insert("help");
    trie.insert("cgraph");    // 插入几个单词

    CGRAPH_ECHO("find [hello] result is : [%i]", trie.find("hello"));
    CGRAPH_ECHO("find [cgraph] result is : [%i]", trie.find("cgraph"));

    trie.eraser("hello");    // 删去hello信息
    CGRAPH_ECHO("eraser [hello], then find it, result is : [%i]", trie.find("hello"));

    trie.insert("hello");    // 重新插入hello信息
    CGRAPH_ECHO("insert [hello] again, then find it, result is : [%i]", trie.find("hello"));
}


void tutorial_trie_v2() {
    UTrieV2 trieV2;
    trieV2.insert("hello");
    trieV2.insert("help");
    trieV2.insert("cgraph");    // 插入几个单词

    CGRAPH_ECHO("find [hello] result is : [%i]", trieV2.find("hello"));
    CGRAPH_ECHO("find [cgraph] result is : [%i]", trieV2.find("cgraph"));

    trieV2.eraser("hello");    // 删去hello信息
    CGRAPH_ECHO("eraser [hello], then find it, result is : [%i]", trieV2.find("hello"));

    trieV2.insert("hello");    // 重新插入hello信息
    CGRAPH_ECHO("insert [hello] again, then find it, result is : [%i]", trieV2.find("hello"));
}


int main() {
    printf("----------------- trie -----------------\n");
    tutorial_trie();
    printf("----------------- trie v2 -----------------\n");
    tutorial_trie_v2();
    return 0;
}
