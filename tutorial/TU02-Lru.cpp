/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: TU02-Lru.cpp
@Time: 2021/9/19 12:43 上午
@Desc: 本例主要演示，Lru工具的使用方法
***************************/

#include "../src/CGraph.h"

using namespace CGraph;

void tutorial_lru() {
    ULru<int, std::string> lru;
    lru.setCapacity(3);    // 设置容量信息

    lru.put(1, "one");
    lru.put(2, "two");
    lru.put(3, "three");
    lru.put(4, "four");    // 此时，key=1的节点，已经被在缓存中删除了
    lru.put(5, "five");

    std::string val = lru.get(4);
    CGRAPH_ECHO("value is : [%s]", val.c_str());
}


int main() {
    tutorial_lru();
    return 0;
}
