/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParamManager.h
@Time: 2021/6/11 6:59 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPARAMMANAGER_H
#define CGRAPH_GPARAMMANAGER_H

#include <unordered_map>
#include <string>

#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"
#include "GParam.h"

CGRAPH_NAMESPACE_BEGIN

class GParamManager : public GParamObject {
public:
    /**
     * 创建一个特定类型的参数
     * @tparam T
     * @param key
     * @return
     */
    template<typename T, std::enable_if_t<std::is_base_of_v<GParam, T>, int> = 0>
    CSTATUS create(const std::string& key);

    /**
     * 获取一个特定类型的参数
     * @param key
     * @return
     */
    GParamPtr get(const std::string& key);


protected:
    explicit GParamManager();
    ~GParamManager() override;
    CSTATUS init() final;
    CSTATUS deinit() final;
    void reset();


private:
    std::unordered_map<std::string, GParamPtr> params_map_;           // 记录param信息的hash表
    std::shared_mutex lock_;                                          // 读写锁
    bool is_init_ { false };                                          // 标记是否初始化结束

    friend class GPipeline;
};

using GParamManagerPtr = GParamManager *;

CGRAPH_NAMESPACE_END

#include "GParamManager.inl"


#endif //CGRAPH_GPARAMMANAGER_H
