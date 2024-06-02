#include "../src/CGraph.h"
#include "unistd.h"
using namespace CGraph;

volatile int flag[3] = {0,0,0};
#define LOOP 1000

class MyNode1 : public CGraph::GNode {
public:
    CStatus run() override {
        float a = 1.1;
        float b = 2.2;
        float c = 3.3;
        for (int i = 0; i < 10; i++) {
            a = b*c;
            b++;
        }
        __sync_fetch_and_add(&flag[1], 1);
        return CStatus();
    }
};

class MyNode2 : public CGraph::GNode {
public:
    CStatus run() override {
        int j = -1;
        int k = 0;
        int t = 0;
        for (int i = 0; i < 10; i++) {
            t = k;
            k = j;
            j = t;
        }
        __sync_fetch_and_add(&flag[0], 1);
        return CStatus();
    }
};

class MyNode3 : public CGraph::GNode {
public:
    CStatus run() override {
        long a = 4;
        long b = 5;
        long c = 6;
        for (int i = 0; i < 10; i++) {
            a = a*a -1;
            b = b*b -4;
            c = c*c -3;
        }
        __sync_fetch_and_add(&flag[2], 1);
        return CStatus();
    }
};

int main() {
    struct timespec ts;
    struct timespec ts2;
    int time[3] = {0,0,0};
    for (int i= 0; i < 10000; i++)
    {   
        /* 创建一个流水线，用于设定和执行流图信息 */
        GPipelinePtr pipeline = GPipelineFactory::create();
        GElementPtr a, b, c = nullptr;

        UThreadPoolConfig config;
        config.default_thread_size_   = 2;
        config.secondary_thread_size_ = 0;
        pipeline->setUniqueThreadPoolConfig(config);    // 设置3个线程执行

        //计时 1
        clock_gettime(CLOCK_REALTIME, &ts);

        int loop=LOOP;
        while(loop--)
        {
            /* 注册节点之间的依赖关系 */
            pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
            pipeline->registerGElement<MyNode2>(&b, {}, "nodeB");
            pipeline->registerGElement<MyNode3>(&c, {}, "nodeC");
            // 等待任务执行完成
            // ...
        }
        pipeline->process();
        while(flag[0]!=LOOP || flag[1]!=LOOP || flag[2]!=LOOP) {
            sleep(0);
        }
    
        //计时2
        clock_gettime(CLOCK_REALTIME, &ts2);

        GPipelineFactory::remove(pipeline);
        flag[0] = 0;
        flag[1] = 0;
        flag[2] = 0;

        time[0] = (ts2.tv_sec-ts.tv_sec)*1000000+(ts2.tv_nsec/1000)-(ts.tv_nsec/1000);
        time[1]+=time[0];//sum
        time[2] =time[1]/(i+1);//avg
        printf("[loop:%04d]cur_time:%04d us,avg_time:%04d us\n", i+1,time[0], time[2]);
    }
    
    sleep(1);

    return 0;
}
