/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T08-Template.cpp
@Time: 2021/9/18 9:52 下午
@Desc: 
***************************/

#include "MyGNode/MyTemplateNode.h"

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
    GPipelineFactory::destroy(pipeline);
}


int main() {
    tutorial_template();
    return 0;
}
