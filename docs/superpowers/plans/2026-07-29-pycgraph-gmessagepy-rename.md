# PyCGraph `GMessagePy` 改名实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**目标：** 将 pycgraph 纯 Python 消息公开接口从 `GMessage` 完整改名为
`GMessagePy`，覆盖实现、pybind 导出、测试、tutorial、打包验证和当前有效文档。

**架构：** 保持 `_pycgraph_message.py`、manager、queue 和全部消息热路径不变，
只修改 façade class、公开类型元数据和 pybind 顶层导出名称。setuptools 模块
清单不需要修改，通过 wheel 构建、内容检查和干净环境集成测试验证打包结果。

**技术栈：** Python 3.8+、pybind11、setuptools、`unittest`。

## 全局约束

- 内部入口文件必须继续命名为 `python/src/_pycgraph_message.py`。
- 公开接口只保留 `pycgraph.GMessagePy`，不保留 `pycgraph.GMessage` 别名。
- 嵌套公开类型必须命名为 `GMessagePy.PushStrategy` 和 `GMessagePy.Error`。
- 消息方法、参数、返回值、异常行为、队列语义和 runtime 热路径保持不变。
- `python/setup.py` 和 `python/pyproject.toml` 继续打包 `_pycgraph_message`。
- 不修改现有 C++ `CGraph::GMessage<T>` runtime。

---

### Task 1：用测试定义新的公开接口

**文件：**

- 修改：`python/tests/test_pycgraph_message.py`
- 修改：`python/tests/test_pycgraph_integration.py`

**接口：**

- 使用：现有 `_pycgraph_message` 内部模块。
- 产出：测试要求 `_pycgraph_message.GMessagePy` 和
  `pycgraph.GMessagePy` 存在，并要求 `pycgraph.GMessage` 不存在。

- [ ] **Step 1：修改纯 Python façade 测试**

  将入口模块导入改为：

  ```python
  from _pycgraph_message import (
      GMessagePy,
      _LocalMessageQueue,
      _LocalMessageManager,
      PyCGraphException,
      _PushStrategy,
  )
  ```

  将 `GMessageFacadeTest` 改为 `GMessagePyFacadeTest`，该测试类中所有
  `GMessage` 调用改为 `GMessagePy`，并断言：

  ```python
  self.assertEqual("pycgraph", GMessagePy.__module__)
  self.assertEqual(
      "GMessagePy.PushStrategy",
      GMessagePy.PushStrategy.__qualname__,
  )
  self.assertEqual("GMessagePy.Error", GMessagePy.Error.__qualname__)
  ```

- [ ] **Step 2：修改安装包集成测试**

  setUp/tearDown 和消息调用统一使用 `self.pycgraph.GMessagePy`。公开接口
  测试必须包含：

  ```python
  self.assertTrue(hasattr(self.pycgraph, "GMessagePy"))
  self.assertFalse(hasattr(self.pycgraph, "GMessage"))
  ```

- [ ] **Step 3：运行纯 Python 测试并确认预期失败**

  运行：

  ```bash
  python3.13 -m unittest \
    python.tests.test_pycgraph_message.GMessagePyFacadeTest -v
  ```

  预期：导入阶段失败，原因是 `_pycgraph_message` 尚未提供
  `GMessagePy`，证明测试能够捕获本次改名。

### Task 2：修改实现和 pybind 公开导出

**文件：**

- 修改：`python/src/_pycgraph_message.py`
- 修改：`python/PyCGraph.cpp`
- 测试：`python/tests/test_pycgraph_message.py`

**接口：**

- 使用：现有 `_LocalMessageManager`、`_PushStrategy` 和
  `PyCGraphException`。
- 产出：`_pycgraph_message.GMessagePy` 与顶层
  `pycgraph.GMessagePy`。

- [ ] **Step 1：完成最小 Python façade 改名**

  在 `_pycgraph_message.py` 中进行以下精确修改：

  ```python
  class GMessagePy:
      """Static façade for single-process Python message operations."""
  ```

  strategy 参数错误文本改为：

  ```python
  "message push strategy must be GMessagePy.PushStrategy"
  ```

  公开元数据改为：

  ```python
  GMessagePy.__module__ = "pycgraph"
  _PushStrategy.__qualname__ = "GMessagePy.PushStrategy"
  PyCGraphException.__qualname__ = "GMessagePy.Error"
  ```

  文件头说明改为 `pycgraph.GMessagePy`。所有 static method 逻辑不改。

- [ ] **Step 2：修改 pybind11 顶层导出**

  保持内部 module import 不变，只修改导出 attribute：

  ```cpp
  py::module_ message_module = py::module_::import("_pycgraph_message");
  cg.attr("GMessagePy") = message_module.attr("GMessagePy");
  ```

- [ ] **Step 3：运行纯 Python message 测试**

  运行：

  ```bash
  python3.13 -m unittest python.tests.test_pycgraph_message -v
  ```

  预期：全部通过，无 warning；queue、manager 和 façade 行为均未变化。

### Task 3：更新 tutorial 及当前有效文档

**文件：**

- 修改：`python/tutorial/T16-MessageSendRecv.py`
- 修改：`python/tutorial/T17-MessagePubSub.py`
- 修改：`python/tutorial/MyGNode/MySendMessageNode.py`
- 修改：`python/tutorial/MyGNode/MyRecvMessageNode.py`
- 修改：`python/tutorial/MyGNode/MyPubMessageNode.py`
- 修改：`python/tutorial/MyGNode/MySubMessageNode.py`
- 修改：`docs/superpowers/specs/2026-07-25-pycgraph-local-message-design.md`
- 修改：`docs/superpowers/plans/2026-07-26-pycgraph-local-message.md`
- 修改：`docs/superpowers/plans/2026-07-27-pycgraph-message-module-split.md`

**接口：**

- 使用：`from pycgraph import GMessagePy`。
- 产出：tutorial 和当前设计/计划文档只使用新的纯 Python API 名称；
  C++ `GMessage` 类型名保持不变。

- [ ] **Step 1：更新所有 Python tutorial**

  将六个 tutorial 文件中的：

  ```python
  from pycgraph import GMessage
  ```

  改为：

  ```python
  from pycgraph import GMessagePy
  ```

  同时将方法调用、`PushStrategy` 和 `Error` 引用统一改为
  `GMessagePy`，不修改 topic、capacity、timeout 或 pipeline 行为。

- [ ] **Step 2：更新当前消息设计和计划文档**

  将文档中描述 pycgraph 纯 Python 公开接口的 `GMessage` 改为
  `GMessagePy`，包括导入示例、类型元数据、异常类型和测试说明。以下
  C++ 标识必须保持原名：

  ```text
  CGraph::GMessage<T>
  GMessageManager
  GMessage.h
  ```

- [ ] **Step 3：检查 Python 范围内的残留名称**

  运行：

  ```bash
  rg -n '\bGMessage\b' \
    python/src python/tests python/tutorial python/PyCGraph.cpp \
    python/setup.py python/pyproject.toml
  ```

  预期：只允许集成测试中验证旧属性不存在的
  `hasattr(self.pycgraph, "GMessage")`；其余 Python API 引用必须全部
  使用 `GMessagePy`。

### Task 4：验证打包和完整行为

**文件：**

- 验证：`python/setup.py`
- 验证：`python/pyproject.toml`
- 验证：`python/tests/test_pycgraph_integration.py`
- 验证：`python/tests/test_message_tutorials.py`

**接口：**

- 使用：setuptools 中保持不变的 `_pycgraph_message` 模块清单。
- 产出：包含 `_pycgraph_message.py` 和 `pycgraph` 扩展的 wheel，且
  安装后仅公开 `GMessagePy`。

- [ ] **Step 1：构建 wheel**

  从 `python/` 目录运行：

  ```bash
  python3.13 -m pip wheel . \
    --no-build-isolation --no-deps \
    --wheel-dir /tmp/pycgraph-gmessagepy-wheel
  ```

  预期：构建成功，证明两处 setuptools 配置仍能找到
  `_pycgraph_message.py`。

- [ ] **Step 2：检查 wheel 内容**

  运行：

  ```bash
  python3.13 -m zipfile -l \
    /tmp/pycgraph-gmessagepy-wheel/pycgraph-*.whl
  ```

  预期：列表包含 `_pycgraph_message.py`、
  `_pycgraph_message_primitives.py`、`_pycgraph_message_manager.py` 和
  `pycgraph` 平台扩展，不包含 `_pycgraph_message_py.py`。

- [ ] **Step 3：在干净虚拟环境中运行集成测试**

  创建 `/tmp/pycgraph-gmessagepy-venv`，安装新 wheel 后运行：

  ```bash
  PYCGRAPH_RUN_INTEGRATION=1 \
    /tmp/pycgraph-gmessagepy-venv/bin/python \
    python/tests/test_pycgraph_integration.py -v
  ```

  预期：全部通过，`GNode`、`GPipeline`、`GMessagePy` 可用，
  `GMessage` 不存在。

- [ ] **Step 4：运行 tutorial 集成测试**

  运行：

  ```bash
  PYCGRAPH_RUN_INTEGRATION=1 \
    /tmp/pycgraph-gmessagepy-venv/bin/python \
    python/tests/test_message_tutorials.py -v
  ```

  预期：T16 和 T17 全部通过，收发与发布订阅输出保持原行为。

- [ ] **Step 5：最终静态检查**

  运行：

  ```bash
  git diff --check
  git status --short
  rg -n '_pycgraph_message_py|\bGMessage\b' \
    python docs/superpowers/specs docs/superpowers/plans
  ```

  预期：无 `_pycgraph_message_py`；独立 `GMessage` 只存在于旧接口缺失
  断言、C++ 类型说明和本次改名迁移说明中。
