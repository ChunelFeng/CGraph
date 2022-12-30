/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T08-Template.cpp
@Time: 2021/9/18 9:52 下午
@Desc: 本例主要演示，注册节点的时候，传入模板节点类型的情况
***************************/

#include "MyGNode/MyTemplateNode.h"

using namespace CGraph;

void tutorial_template() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GTemplateNodePtr<int, float> a = nullptr;
    GTemplateNodePtr<int, float> b = nullptr;
    GTemplateNodePtr<int> c = nullptr;

    /**
     * 注册几个模板节点
     * 可以根据 MyTemplateNode构造函数的不同，而实现不同的构造方式
     */
    pipeline->registerGElement<MyTemplateNode<int, float>>(&a, {}, 3, 3.5f);
    pipeline->registerGElement<MyTemplateNode<int, float>>(&b, {a},5, 3.75f);
    pipeline->registerGElement<MyTemplateNode<int>>(&c, {b}, 8);

    pipeline->process();    // 运行pipeline
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_template();
    return 0;
}
