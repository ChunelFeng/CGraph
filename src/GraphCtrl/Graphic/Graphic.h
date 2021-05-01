/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphCtrl.h
@Time: 2021/4/26 9:16 下午
@Desc: 
***************************/


#ifndef CGRAPH_GRAPHIC_H
#define CGRAPH_GRAPHIC_H

#include <list>
#include <queue>
#include <vector>

#include "../../CObject/CObject.h"
#include "../GraphNode/GraphNode.h"
#include "../GraphDefine.h"


class Graphic : public CObject {
public:
    /**
    * 构造函数
    */
    Graphic();

    /**
     * 初始化函数
     */
    CSTATUS init() override;

    /**
     * 流程处理函数，必须实现
     * @return
     */
    CSTATUS run() override;

    /**
     * 逆初始化函数
     * @return
     */
    CSTATUS deinit() override;

    /**
     * 析构函数
     */
    ~Graphic() override;

public:
    CSTATUS addNode(GraphNode* object);

private:
    std::queue<GraphNode *> que_;    // 计算后的数据
    std::list<GraphNode *> nodes_;    // 插进来的数据
};


#endif //CGRAPH_GRAPHIC_H
