/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyConnAspect.h
@Time: 2021/10/4 12:12 下午
@Desc: 
***************************/

#ifndef CGRAPH_MYCONNASPECT_H
#define CGRAPH_MYCONNASPECT_H

#include "../../src/CGraph.h"
#include "../MyParams/MyConnParam.h"

class MyConnAspect : public CGraph::GAspect {
public:
    CStatus beginInit() override {
        auto* param = this->getAParam<MyConnParam>();    // 注意，这里是AParam，表示的是切面自己的参数，不是GParam
        if (param) {
            // 如果传入类型不匹配，则返回param值为空
            mockConnect(param->ip_, param->port_);
        }

        return CStatus();
    }

    CVoid finishDestroy(const CStatus& curStatus) override {
        auto* param = this->getAParam<MyConnParam>();
        if (param) {
            mockDisconnect(param->ip_, param->port_);
        }
    }

protected:
    /** 模拟连接功能 */
    CVoid mockConnect(const std::string& ip, short port) {
        CGraph::CGRAPH_ECHO("----> [MyConnAspect] [%s : %d] has been connected ...", ip.c_str(), port);
        conn_status_ = true;
    }

    /** 模拟断开连接功能 */
    CVoid mockDisconnect(const std::string& ip, short port) {
        if (conn_status_) {
            CGraph::CGRAPH_ECHO("----> [MyConnAspect] [%s : %d] has been disconnected ...", ip.c_str(), port);
            conn_status_ = false;
        } else {
            CGraph::CGRAPH_ECHO("----> [MyConnAspect] [%s : %d] is already been disconnected ...", ip.c_str(), port);
        }
    }

private:
    bool conn_status_ { false };                    // 表示连接状态
};

#include "../../src/CGraph.h"



#endif //CGRAPH_MYCONNASPECT_H
