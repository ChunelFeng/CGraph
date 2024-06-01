/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyMessageParam.h
@Time: 2022/11/3 23:45
@Desc: 
***************************/

#ifndef CGRAPH_MYMESSAGEPARAM_H
#define CGRAPH_MYMESSAGEPARAM_H

#include "CGraph.h"

struct MyMessageParam : public CGraph::GMessageParam {
    int num = 0;
    std::string info;

    explicit MyMessageParam() = default;

    /**
     * 注意：
     * 使用 GMessageParam 类型参数的时候，建议实现构造拷贝 和 赋值拷贝函数
     * 否则，针对指针类型变量，可能会出现深浅拷贝的问题
     */
    MyMessageParam(const MyMessageParam& param) : CGraph::GMessageParam(param) {
        /** 针对通过 unique_ptr<value> 获取的方式，推荐实现构造拷贝函数 */
        this->info = param.info;
        this->num = param.num;
    }

    MyMessageParam& operator=(const MyMessageParam& param) {
        /** 针对通过 value 获取的方式，推荐实现赋值拷贝函数 */
        if (this == &param) {
            return *this;
        }

        this->info = param.info;
        this->num = param.num;
        return (*this);
    }
};

#endif //CGRAPH_MYMESSAGEPARAM_H
