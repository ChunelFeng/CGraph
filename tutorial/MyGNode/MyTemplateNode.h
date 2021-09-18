/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyTemplateNode.h
@Time: 2021/9/18 9:52 下午
@Desc:
***************************/

#ifndef CGRAPH_MYTEMPLATENODE_H
#define CGRAPH_MYTEMPLATENODE_H

#include "../../src/CGraph.h"

template <typename T>
class MyTemplateNode : public GNode {
public:
    CSTATUS run () override {
        if (strcmp(typeid(val_).name(), "c") == 0) {
            CGRAPH_ECHO("This node template type is [char].");    // 对应模板类型是 char 类型
        } else if (strcmp(typeid(val_).name(), "f") == 0) {
            CGRAPH_ECHO("This node template type is [float].");    // 对应模板类型是 float 类型
        } else if (strcmp(typeid(val_).name(), "i") == 0) {
            CGRAPH_ECHO("This node template type is [int].");    // 对应模板类型是 int 类型
        } else {
            CGRAPH_ECHO("This node template type is others.");
        }

        /* 模板类型节点，还可以和GParam逻辑联动，实现更多功能 */
        return STATUS_OK;
    }

private:
    T val_;    // 模板类型成员变量
};


#endif //CGRAPH_MYTEMPLATENODE_H
