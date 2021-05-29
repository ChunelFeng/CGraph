/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphNode.h
@Time: 2021/4/26 9:29 下午
@Desc: GraphNode类有纯虚函数run，无法直接new。继承本类，必须实现run方法
***************************/

#ifndef CGRAPH_GRAPHNODE_H
#define CGRAPH_GRAPHNODE_H

#include <atomic>
#include <set>

#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"

class GraphNode : public CObject {
public:
    /**
     * 获取节点name信息
     * name信息可以重复
     */
    const std::string& getName() const;

    /**
     * 获取节点session信息
     * session信息不可重复
     */
    const std::string& getSession() const;

protected:
    explicit GraphNode();    // GraphNode 节点，不允许自己new

    ~GraphNode() override;

    CSTATUS init() override;

    CSTATUS deinit() override;

    /**
     * 实现拷贝构造函数
     * @param node
     */
    GraphNode(const GraphNode& node);

    /**
     * 赋值构造函数
     * @param node
     * @return
     */
    GraphNode& operator=(const GraphNode& node);

    /**
     * 设置name值
     * @param name
     */
    void setName(const std::string& name = "");

    /**
     * 设置自循环次数信息
     * @param runTimes
     */
    void setRunTimes(int runTimes = 1);

    /**
     * run方法执行之前的执行函数
     * @return
     */
    CSTATUS beforeRun();

    /**
     * run方法执行之后的执行函数
     * @return
     */
    CSTATUS afterRun();

    /**
     * 线程池中的运行函数，依次执行beforeRun，run和afterRun方法，
     * 其中有任何返回值问题，则直接返回
     * @return
     */
    CSTATUS process();

    /**
     * 判定node是否可以运行
     * 可执行的条件为：自身未被执行且依赖节点全部被执行
     * nodiscard 表示返回值必须被判断
     * @return
     */
    [[nodiscard]]bool isRunnable() const;

    /**
     * 判定node是否可以和前面节点一起执行
     * @return
     */
    [[nodiscard]]bool isLinkable() const;

    /**
     * 生成唯一id信息
     */
    static std::string _generateSession();

    struct LoopCtrl {
        /* GraphNode内部类，用于管控循环逻辑 */
        int loop_;
        GraphNode* head_;
        GraphNode* tail_;
        std::string session_;

        LoopCtrl(std::string sess) {
            loop_ = 1;
            head_ = nullptr;
            tail_ = nullptr;
            session_ = sess;
        }

        LoopCtrl() {
            loop_ = 1;
            head_ = nullptr;
            tail_ = nullptr;
        }

        LoopCtrl(const LoopCtrl& loopCtrl) {
            this->loop_ = loopCtrl.loop_;
            this->head_ = loopCtrl.head_;
            this->tail_ = loopCtrl.tail_;
            this->session_ = loopCtrl.session_;
        }

        void setInfo(GraphNode* start, GraphNode* end, int loop, std::string session = "") {
            this->head_ = start;
            this->tail_ = end;
            this->loop_ = loop;
            this->session_ = session;
        }

        bool enableLink() const {
            /* 循环为1次，切没有next节点的时候，可以与其他节点link，并且没有loopSession信息 */
            return 1 == loop_ && nullptr == head_ && nullptr == tail_ && session_.empty();
        }
    };

private:
    bool done_ { false };                     // 判定被执行结束
    bool linkable_ { false };                 // 判定是否可以连通计算
    int run_times_ {1};                       // 节点执行次数
    std::string name_;                        // 节点名称
    std::string session_;                     // 节点唯一id信息
    std::set<GraphNode *> run_before_;        // 被依赖的节点
    std::set<GraphNode *> dependence_;        // 依赖的节点信息
    std::atomic<int> left_depend_{0};         // 当 left_depend_ 值为0的时候，即可以执行该node信息
    LoopCtrl loop_ctrl_;

    /* 设置友元类 */
    friend class Graphic;
    friend class GraphThreadPool;
    friend class GraphNodeManager;
    friend class GraphNodeCluster;
};


#endif //CGRAPH_GRAPHNODE_H
