/***************************
@Author: Lowpower
@Contact: szq123456123@gmail.com
@File: E05-Dynamic.cpp
@Time: 2024/5/10 16:35
@Desc: 本example主要展示，根据pb配置动态生成流图, 编译运行需要c++17
***************************/

#include <any>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

#include "CGraph.h"

/// 用pb性能好一些,例子里用struct替代
struct Attribute {
  std::string name;
  std::variant<std::string, int64_t, double> oneof_value;
};

struct NodeDef {
  std::string name;
  std::string alias;
  std::vector<Attribute> attributes;
};

struct Relationship {
  std::string name;
  std::vector<std::string> dependencies;
};

struct DynamicGraphConf {
  int64_t id;
  std::string desc;
  std::vector<NodeDef> node_defs;
  std::vector<Relationship> relationships;
};

class DynamicNode;
using DynamicNodePtr = std::shared_ptr<DynamicNode>;
using ElementBuilderFunc = std::function<CGraph::GElementPtr(CGraph::GPipelinePtr, const std::unordered_map<std::string, std::any>&)>;
using NodeRegistFunc = std::function<void(CGraph::GPipelinePtr)>;

class DynamicNode {
 public:
  explicit DynamicNode(const std::string& name, const std::string& alias, ElementBuilderFunc buildFunc)
      : name_(name), alias_(alias), buildFunc_(buildFunc) {
  }
  template <typename T>
  void AddAttribute(const std::string& key, const T& value) {
    attributes_[key] = value;
  }
  inline CGraph::GElementPtr Build(CGraph::GPipelinePtr pipeline) {
    return buildFunc_(pipeline, attributes_);
  }
  inline const std::string& GetName() const {
    return name_;
  }

 private:
  std::string name_;
  std::string alias_;
  std::unordered_map<std::string, std::any> attributes_;
  std::vector<DynamicNodePtr> dependency_;
  ElementBuilderFunc buildFunc_;
};

class DynamicGraph {
 public:
  struct NodeRegistInfo {
    DynamicNodePtr dynamicNode;
    std::vector<std::string> dependencies;
  };
  explicit DynamicGraph(const std::string name) : name_(name) {
  }
  bool AddNode(DynamicNodePtr dynamicNode) {
    const auto& name = dynamicNode->GetName();
    if (nodeMapping_.find(name) != nodeMapping_.end()) {
      std::cout << "DynamicNode already exists:" << name << std::endl;
      return false;
    }
    nodeMapping_.insert(std::make_pair(name, NodeRegistInfo{dynamicNode}));
    nodeList_.push_back(name);
    return true;
  }
  bool AddNodeDep(const std::string& name, const std::vector<std::string>& deps) {
    if (nodeMapping_.find(name) != nodeMapping_.end()) {
      nodeMapping_[name].dependencies = deps;
    } else {
      std::cout << "AddNodeDep DynamicNode not exists:" << name << std::endl;
      return false;
    }
    return true;
  }
  CGraph::GPipelinePtr Build() {
    /* 创建一个流水线，用于设定和执行流图信息 */
    CGraph::GPipelinePtr pipeline = CGraph::GPipelineFactory::create();
    std::unordered_map<std::string_view, CGraph::GElementPtr> eleMapping;
    for (const auto& node : nodeList_) {
      const auto& nodeInfo = nodeMapping_[node];
      auto complateNode = nodeInfo.dynamicNode->Build(pipeline);
      const auto& name = nodeInfo.dynamicNode->GetName();
      eleMapping[name] = complateNode;
      std::set<CGraph::GElementPtr> dependElements;
      for (const auto& dependency : nodeInfo.dependencies) {
        dependElements.insert(eleMapping[dependency]);
      }
      pipeline->registerGNode(&complateNode, dependElements, name);
    }
    // 返回可执行的pipelineptr
    pipeline->setName(name_);
    std::cout << "DynamicGraph::Build: " << name_ << std::endl;
    pipeline->dump();
    return pipeline;
  }
  inline const std::string& GetName() const {
    return name_;
  }

 private:
  std::string name_;
  // 节点列表
  std::unordered_map<std::string, NodeRegistInfo> nodeMapping_;
  std::vector<std::string> nodeList_;
};

std::unordered_map<std::string, ElementBuilderFunc> dynamic_nodes_;
std::unordered_map<int64_t, std::shared_ptr<DynamicGraph>> dynamic_graphs_;

bool example_dynamic_graph(const DynamicGraphConf& graphpb) {
  if (dynamic_graphs_.find(graphpb.id) != dynamic_graphs_.end()) {
    std::cout << "DynamicGraph already exists: " << graphpb.id << std::endl;
    return false;
  }
  auto graph = std::make_shared<DynamicGraph>(graphpb.desc);
  for (const auto& nodedef : graphpb.node_defs) {
    if (dynamic_nodes_.find(nodedef.alias) == dynamic_nodes_.end()) {
      std::cout << "DynamicNode " << nodedef.alias << " is noexists" << std::endl;
      return false;
    }
    auto node = std::make_shared<DynamicNode>(nodedef.name, nodedef.alias, dynamic_nodes_.at(nodedef.alias));
    for (const auto& attribute : nodedef.attributes) {
      if (attribute.oneof_value.index() == 0) {
        node->AddAttribute(attribute.name, std::get<0>(attribute.oneof_value));
      } else if (attribute.oneof_value.index() == 1) {
        node->AddAttribute(attribute.name, std::get<1>(attribute.oneof_value));
      } else if (attribute.oneof_value.index() == 2) {
        node->AddAttribute(attribute.name, std::get<2>(attribute.oneof_value));
      }
    }
    if (!graph->AddNode(node)) {
      return false;
    }
  }
  for (const auto& relation : graphpb.relationships) {
    std::vector<std::string> deps;
    for (const auto& dependency : relation.dependencies) {
      deps.push_back(dependency);
    }
    if (!graph->AddNodeDep(relation.name, deps)) {
      return false;
    }
  }
  auto pipeline = graph->Build();
  pipeline->init();
  dynamic_graphs_.insert(std::make_pair(graphpb.id, graph));
  return true;
}

// test
class TestGNode : public CGraph::GNode {
 public:
  CStatus init() override {
    return CStatus();
  }

  CStatus run() override {
    std::cout << "TestGNode run" << std::endl;
    return CStatus();
  }
};

int main() {
  DynamicGraphConf graphpb;
  graphpb.id = 1;
  graphpb.desc = "example_dynamic_graph";
  graphpb.node_defs.push_back(NodeDef());
  graphpb.node_defs[0].name = "example_dynamic_node";
  graphpb.node_defs[0].alias = "example_dynamic_node";
  graphpb.node_defs[0].attributes.push_back(Attribute());
  dynamic_nodes_.insert(std::make_pair("example_dynamic_node", [](CGraph::GPipelinePtr pipeline, const std::unordered_map<std::string, std::any>&) -> CGraph::GElementPtr {
    CGraph::GElementPtr e = pipeline->createGNode<TestGNode>(CGraph::GNodeInfo("example_dynamic_node"));
    return e;
  }));
  example_dynamic_graph(graphpb);
  return 0;
}