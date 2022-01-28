/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T08-Template.cpp
@Time: 2021/9/18 9:52 下午
@Desc: 本例主要演示，注册节点的时候，传入模板类型的情况
***************************/

#include "MyGNode/MyTemplateNode.h"

using namespace CGraph;

void tutorial_template() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    /**
     * 依次注册 int/char/float/string 类型的模板节点 a/b/c/d
     * 然后依次执行，查看效果
     */
    pipeline->registerGElement<MyTemplateNode<int>>(&a);
    pipeline->registerGElement<MyTemplateNode<char>>(&b, {a});
    pipeline->registerGElement<MyTemplateNode<float>>(&c, {b});
    pipeline->registerGElement<MyTemplateNode<std::string>>(&d, {c});

    pipeline->process();    // 运行pipeline
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_template();
    return 0;
}
