/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectDefine.h
@Time: 2021/10/3 9:44 上午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECTDEFINE_H
#define CGRAPH_GASPECTDEFINE_H

enum class GAspectType {
    BEGIN_INIT = 0,
    FINISH_INIT = 1,
    BEGIN_RUN = 2,
    FINISH_RUN = 3,
    BEGIN_DEINIT = 4,
    FINISH_DEINIT = 5
};

#endif //CGRAPH_GASPECTDEFINE_H
