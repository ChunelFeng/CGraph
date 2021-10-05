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
#include "../MyGParam/MyConnParam.h"

class MyConnAspect : public GAspect {
public:
    CSTATUS beginInit() override {
        auto* param = this->getParam<MyConnParam>();
        if (param) {
            // 如果传入类型不匹配，则返回param值为空
            mockConnect(param->ip, param->port);
        }

        return STATUS_OK;
    }

    void finishDeinit(CSTATUS status) override {
        auto* param = this->getParam<MyConnParam>();
        if (param) {
            mockDisconnect(param->ip, param->port);
        }
    }

protected:
    /** 模拟连接功能 */
    void mockConnect(const std::string& ip, short port) {
        CGRAPH_ECHO("----> [MyConnAspect] [%s : %d] has been connected ...", ip.c_str(), port);
        conn_status_ = true;
    }

    /** 模拟断开连接功能 */
    void mockDisconnect(const std::string& ip, short port) {
        if (conn_status_) {
            CGRAPH_ECHO("----> [MyConnAspect] [%s : %d] has been disconnected ...", ip.c_str(), port);
            conn_status_ = false;
        } else {
            CGRAPH_ECHO("----> [MyConnAspect] [%s : %d] is already been disconnected ...", ip.c_str(), port);
        }
    }

private:
    bool conn_status_ { false };                    // 表示连接状态
};

#include "../../src/CGraph.h"



#endif //CGRAPH_MYCONNASPECT_H
