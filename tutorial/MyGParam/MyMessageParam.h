/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyMessageParam.h
@Time: 2022/11/3 23:45
@Desc: 
***************************/

#ifndef CGRAPH_MYMESSAGEPARAM_H
#define CGRAPH_MYMESSAGEPARAM_H

#include "../../src/CGraph.h"

struct MyMessageParam : public CGraph::GMessageParam {
    int num;
    std::string info;

    MyMessageParam& operator=(const MyMessageParam& param) {
        /**
         * 注意：
         * 使用 GMessageParam 类型参数的时候，建议 实现赋值拷贝函数
         * 否则，针对指针类型变量，可能会出现深浅拷贝的问题
         */
        if (this == &param) {
            return *this;
        }

        this->info = param.info;
        this->num = param.num;
        return (*this);
    }
};

#endif //CGRAPH_MYMESSAGEPARAM_H
