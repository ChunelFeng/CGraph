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
    ULru<int, std::string, 3> lru;

    lru.put(1, "one");
    lru.put(2, "two");
    lru.put(3, "three");
    lru.put(4, "four");    // 此时，key=1的节点，已经被在缓存中删除了
    lru.put(5, "five");

    std::string val;
    int key = 5;
    // 返回值的 true的情况下，表示获取成功。否则表示未获取到 value 的值
    if (lru.get(key, val)) {
        CGRAPH_ECHO("key = %d, value is : [%s]", key, val.c_str());
    } else {
        CGRAPH_ECHO("[%d] no get value", key);
    }

    key = 6;
    if (lru.get(key, val)) {
        CGRAPH_ECHO("key = %d, value is : [%s]", key, val.c_str());
    } else {
        CGRAPH_ECHO("[%d] no get value", key);
    }
}


int main() {
    tutorial_lru();
    return 0;
}
