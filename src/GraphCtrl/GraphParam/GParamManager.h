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
#include <vector>
#include <string>
#include <mutex>

#include "../GraphObject.h"
#include "../GraphManager.h"
#include "GParam.h"

CGRAPH_NAMESPACE_BEGIN

class GParamManager : public GParamObject,
                      public GraphManager<GParam> {
public:
    /**
     * 创建一个特定类型的参数
     * @tparam T
     * @param key
     * @param backtrace 是否开启记录调用链路功能
     * @return
     */
    template<typename T, c_enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    CStatus create(const std::string& key, CBool backtrace = false);

    /**
     * 获取一个特定类型的参数
     * @tparam T
     * @param key
     * @return
     */
    template<typename T, c_enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    T* get(const std::string& key);

    /**
     * 删除特定的param信息
     * @param key
     * @return
     */
    CStatus removeByKey(const std::string& key);

    /**
     * 获取所有的 GParam keys 信息
     * @return
     */
    std::vector<std::string> getKeys();

protected:
    explicit GParamManager();
    ~GParamManager() override;
    CStatus init() final;
    CStatus destroy() final;

    CStatus clear() final;

    /**
     * 初始化所有的参数信息
     * @return
     */
    CStatus setup();

    /**
     * 重置所有的GParam信息
     * @param curStatus
     * @return
     */
    CVoid resetWithStatus(const CStatus& curStatus);

    CGRAPH_NO_ALLOWED_COPY(GParamManager)

private:
    std::unordered_map<std::string, GParamPtr> params_map_;           // 记录param信息的hash表
    std::mutex mutex_;                                                // 创建param的时候上锁

    friend class GPipeline;
    friend class CAllocator;
    friend class GStorage;

public:
    /// 为 python 版本设定的函数，cpp 的童鞋不需要使用
    CStatus __create_4py(GParamPtr param, const std::string& key);
    GParamPtr __get_4py(const std::string& key);
    CStatus __remove_4py(const std::string& key);
    CBool __has_4py(const std::string& key);
};

using GParamManagerPtr = GParamManager *;

CGRAPH_NAMESPACE_END

#include "GParamManager.inl"

#endif //CGRAPH_GPARAMMANAGER_H
