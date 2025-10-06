/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Storage.cpp
@Time: 2025/10/4 13:43
@Desc: 本例子主要展示保存
***************************/

#include <iostream>
#include <cstdio>

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

    #if __cplusplus >= 201703L
void tutorial_storage_save(const std::string& path) {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    status += pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");
    status += pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    status += pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");
    status += pipeline->save(path);
    if (status.isOK()) {
        std::cout << "pipline have been save to " << path << std::endl;
    } else {
        std::cout << " save pipeline failed ...\n";
    }

    GPipelineFactory::remove(pipeline);    // 析构当前pipeline即可
}

void tutorial_storage_load(const std::string& path) {
    // 需要通过如下方式，提前注册所有的使用到的 meta 信息
    CGRAPH_REGISTER_META_TYPE(MyNode1);
    CGRAPH_REGISTER_META_TYPE(MyNode2);

    GPipelinePtr pipeline = GPipelineFactory::create();    // 创建一个新的pipeline，用于加载
    std::cout << "create another pipeline, and load pipeline for path : " << path << std::endl;
    CStatus status = pipeline->load(path);
    if (status.isErr()) {
        std::cout << status.getInfo() << std::endl;
        return;
    }

    pipeline->process();
    GPipelineFactory::remove(pipeline);

    // 删除存储文件
    std::remove(path.c_str());
}

void tutorial_storage() {
    const std::string& path = "storage.cgraph";
    tutorial_storage_save(path);
    tutorial_storage_load(path);
}
    #else
void tutorial_storage() {
    std::cout << "CGraph storage support cpp17 and upper only \n";
}
    #endif


int main () {
    tutorial_storage();
    return 0;
}
