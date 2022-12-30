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
     * @return
     */
    template<typename T, std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    CStatus create(const std::string& key);

    /**
     * 获取一个特定类型的参数
     * @tparam T
     * @param key
     * @return
     */
    template<typename T, std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    T* get(const std::string& key);

    /**
     * 获取一个特定类型的参数，如果为空的话，则抛出异常信息
     * @tparam T
     * @param key
     * @return
     */
    template<typename T, std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    T* getWithNoEmpty(const std::string& key);


protected:
    explicit GParamManager();
    ~GParamManager() override;
    CStatus init() override;
    CStatus destroy() override;

    /**
     * 清空内部所有参数信息
     */
    CStatus clear() final;

    /**
     * 重置内部所有参数信息
     */
    CStatus reset() override;

    CGRAPH_NO_ALLOWED_COPY(GParamManager)

private:
    std::unordered_map<std::string, GParamPtr> params_map_;           // 记录param信息的hash表
    std::mutex lock_;                                                 // 创建param的时候上锁

    friend class GPipeline;
    friend class UAllocator;
};

using GParamManagerPtr = GParamManager *;

CGRAPH_NAMESPACE_END

#include "GParamManager.inl"


#endif //CGRAPH_GPARAMMANAGER_H
