# PyCGraph `GMessagePy` 改名设计

**日期：** 2026-07-29

## 目标

将纯 Python 消息接口从 `GMessage` 改名为 `GMessagePy`，明确表达其
runtime 实现。今后如果增加基于 C++ 的 Python 消息接口，可以使用
`GMessageCpp` 等独立名称。

## 公开接口

- 使用 `pycgraph.GMessagePy` 替换 `pycgraph.GMessage`。
- 不保留 `GMessage` 兼容别名。
- 所有消息方法、参数、返回值、异常行为、队列语义和 runtime 性能特征
  均保持不变。
- 统一修改嵌套公开类型的元数据：
  - `GMessagePy.PushStrategy`
  - `GMessagePy.Error`

## Python 源码与打包

- 内部入口文件继续使用 `_pycgraph_message.py`，只将其中的公开 façade
  class 从 `GMessage` 改为 `GMessagePy`。
- `_pycgraph_message_primitives` 和 `_pycgraph_message_manager` 保持不变。
  这两个名称描述的是实现职责，不需要携带公开 backend 名称。
- pybind11 继续导入 `_pycgraph_message`，但只向 `pycgraph` 导出
  `GMessagePy`。
- 两处 setuptools 模块清单保持不变，并验证源码包和 wheel 中仍然包含
  `_pycgraph_message`。

## 测试与教程

- 单元测试和集成测试统一导入并使用 `GMessagePy`。
- 集成测试增加 `pycgraph.GMessage` 不存在的断言，防止意外保留兼容别名。
- 所有 Python tutorial 及其辅助 node 统一使用 `GMessagePy`。
- 更新现有消息设计文档和实施计划中的接口名称，使文档记录与代码一致。

## 兼容性与 Runtime 影响

这是一次有意进行的源码不兼容改名。现有调用方需要将 `GMessage` 替换为
`GMessagePy`。

本次修改不会增加 runtime 抽象、backend 自动选择、内存分配、锁、数据
拷贝或热路径分支。模块导入和属性转发仍然只在初始化时执行一次；消息
收发和发布订阅路径保持不变。

## 验证方式

- 先修改测试，确认测试因为 `GMessagePy` 尚未导出而按预期失败。
- 修改生产代码导出名称，并检查保持不变的打包声明。
- 运行消息单元测试、扩展集成测试和 tutorial 测试。
- 通过项目正常的构建和安装测试路径验证 `_pycgraph_message` 仍被正确
  打包，并能够导出 `GMessagePy`。
- 检查 Python 源码、测试、tutorial 以及当前有效的设计和计划文档，
  确认不存在遗漏的独立 `GMessage` 引用。
