/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyVersionParam.h
@Time: 2022/5/29 22:29
@Desc: 
***************************/

#ifndef CGRAPH_MYVERSIONPARAM_H
#define CGRAPH_MYVERSIONPARAM_H

#include "../../src/CGraph.h"

const static char* VERSION_PARAM_KEY = "version";

struct MyVersionParam : public CGraph::GElementParam {
    CVoid clone(CGraph::GPassedParamPtr param) override {
        auto* ptr = dynamic_cast<MyVersionParam *>(param);
        if (nullptr == ptr) {
            return;    // 理论不存在的情况
        }

        this->priority_ = ptr->priority_;
        this->secondary_ = ptr->secondary_;
    }

    int priority_ = 0;    // 表示 主版本号
    int secondary_ = 0;    // 表示 辅版本号
};


#endif //CGRAPH_MYVERSIONPARAM_H
