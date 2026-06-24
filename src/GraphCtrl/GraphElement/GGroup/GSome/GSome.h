/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GSome.h
@Time: 2023/8/20 11:20
@Desc: 
***************************/

#ifndef CGRAPH_GSOME_H
#define CGRAPH_GSOME_H

#include <mutex>
#include <condition_variable>

#include "../GGroup.h"

CGRAPH_NAMESPACE_BEGIN

class GSome : public GGroup {
protected:
    /**
     * 设定 wait_num 个数
     * 当前 group 执行完成 wait_num 个后，就可以继续执行
     * @return
     */
    virtual CSize getWaitNum() = 0;

protected:
    explicit GSome();

    CStatus run() final;

    CBool isSerializable() const final;

    CVoid dump(std::ostream& oss) final;

    CBool isHold() final;

    CStatus checkSuitable() final;

    CGRAPH_NO_ALLOWED_COPY(GSome)

    CStatus addElementEx(GElementPtr element) override;

private:
    CInt wait_num_ {0};                        // 还剩的触发结束的个数
    CStatus cur_status_ ;                      // 记录异步时刻的当前状态信息

    std::mutex lock_;
    std::condition_variable cv_;

    friend class GPipeline;
    friend class GDynamicEngine;
    friend class CAllocator;
    friend class GStorageFactory;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSOME_H
