/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyTemplateAspect.h
@Time: 2022/12/23 22:44
@Desc: 
***************************/

#ifndef CGRAPH_MYTEMPLATEASPECT_H
#define CGRAPH_MYTEMPLATEASPECT_H

#include "../../src/CGraph.h"

template<class ...Args>
class MyTemplateAspect : public CGraph::GTemplateAspect<Args...> {
public:
    explicit MyTemplateAspect(int age, double score) {
        age_ = age;
        score_ = score;
    }

    CStatus beginInit() override {
        CGraph::CGRAPH_ECHO("----> [MyTemplateAspect] [%s] input age is [%d], score is [%lf]",
                            this->getName().c_str(), age_, score_);
        return CStatus();
    }

private:
    int age_;
    double score_;
};

#endif //CGRAPH_MYTEMPLATEASPECT_H
