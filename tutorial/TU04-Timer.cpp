/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: TU04-Timer.cpp
@Time: 2021/12/31 9:12 下午
@Desc: 本例主要演示，Timer的使用方法
***************************/

#include "../src/CGraph.h"

using namespace CGraph;

void tutorial_timer() {
    UTimer timer;
    timer.start(1000, [] {
        CGRAPH_ECHO("Hello, CGraph");
    });

    CGRAPH_SLEEP_MILLISECOND(5500)
    timer.stop();
}


int main() {
    tutorial_timer();
    return 0;
}