# PyCGraph 单进程 Message 功能设计

## 1. 状态

- 日期：2026-07-25
- 状态：已完成讨论，等待书面评审
- 范围：单 Python 进程内的 message 功能

## 2. 背景

CGraph C++ 已提供两类 message 语义：

- `send/recv`：一条消息只由一个 receiver 消费。
- `pub/sub`：一条消息分发到多个 subscriber 的独立队列。

当前 `pycgraph` 没有对应能力。若直接复用 C++ `GMessageManager`，每次消息都需要经过 Python、pybind11、C++ queue，再返回 Python。普通 Python 对象还需要额外的 holder、引用计数和 GIL 生命周期管理。对于仅在 Python node/pipeline 之间传递的消息，这些复杂度没有带来额外业务能力。

本设计在 Python 层直接实现单进程 message，避免每条消息进入 C++。公开语义尽量对齐 CGraph，函数命名使用 Python `snake_case`。

## 3. 目标

1. 支持单进程内跨 node、跨 pipeline 的 `send/recv`。
2. 支持单进程内跨 node、跨 pipeline 的 `pub/sub`。
3. 支持 `WAIT`、`REPLACE`、`DROP` 三种 push strategy。
4. hot path 只传递 Python 对象引用，不序列化、不复制 payload。
5. 保持现有 `pycgraph` 扩展模块名称、目录结构和已有导入方式。
6. 纯 Python message API 通过 `from pycgraph import ...` 公开。
7. 实现保持小而清晰，容易 debug 和维护。

## 4. 非目标

本次不实现：

- 跨进程通信。
- Python message 与 C++ message 互通。
- 序列化或反序列化。
- shared memory、PyZMQ 或其他 IPC backend。
- `asyncio` API。
- MPMC 拓扑。
- payload 类型校验。
- payload 不可变性的 runtime 强制校验。
- message metrics、trace 或 benchmark。
- pipeline destroy 时自动清理 topic。

如果未来增加跨进程能力，应重新设计独立的 IPC 方案，不在当前实现中预埋复杂 transport abstraction。

## 5. 公开 API

### 5.1 类型

```python
class GMessageParam:
    """可选的 marker base class，不参与 runtime 类型校验。"""


class GMessagePushStrategy(Enum):
    WAIT = ...
    REPLACE = ...
    DROP = ...


class GMessageError(RuntimeError):
    """所有 message 错误使用的唯一异常类型。"""
```

`GMessageParam` 用于保持 Python 代码与 CGraph 概念一致。message 系统接受任意 Python 对象，不要求 payload 继承该类型，也不对 `None` 等特殊值增加判断。

### 5.2 Send/Recv

```python
create_message_topic(topic: str, capacity: int) -> None
remove_message_topic(topic: str) -> None

send_message(
    topic: str,
    message: object,
    strategy: GMessagePushStrategy = GMessagePushStrategy.WAIT,
) -> int

recv_message(
    topic: str,
    timeout_ms: Optional[int] = None,
) -> object
```

`send_message()` 返回成功入队数量，只可能为：

- `1`：消息成功写入。
- `0`：使用 `DROP` 且队列已满。

### 5.3 Pub/Sub

```python
bind_message_topic(topic: str, capacity: int) -> int

pub_message(
    topic: str,
    message: object,
    strategy: GMessagePushStrategy = GMessagePushStrategy.WAIT,
) -> int

sub_message(
    conn_id: int,
    timeout_ms: Optional[int] = None,
) -> object

detach_message_subscription(topic: str, conn_id: int) -> None
drop_message_topic(topic: str) -> None
```

`bind_message_topic()` 返回全局单调递增的 `conn_id`。

`pub_message()` 返回本次成功写入的 subscriber 队列数量：

- `WAIT` 完成后，返回当前 subscriber 总数。
- `REPLACE` 中替换旧消息后写入新消息，计为成功。
- `DROP` 中只统计未丢弃消息的 subscriber。
- topic 存在但没有 subscriber 时返回 `0`。

返回值只表示成功入队，不表示消息已经被业务代码消费。

### 5.4 全局清理

```python
clear_messages() -> None
```

### 5.5 使用示例

```python
from pycgraph import (
    GMessageParam,
    GMessagePushStrategy,
    create_message_topic,
    send_message,
    recv_message,
)


class MyMessage(GMessageParam):
    pass


create_message_topic("send-recv", capacity=64)
send_message("send-recv", MyMessage(), GMessagePushStrategy.WAIT)
message = recv_message("send-recv", timeout_ms=1000)
```

```python
from pycgraph import (
    GMessagePushStrategy,
    bind_message_topic,
    pub_message,
    sub_message,
)


conn_id = bind_message_topic("pub-sub", capacity=64)
pub_message("pub-sub", message, GMessagePushStrategy.WAIT)
message = sub_message(conn_id, timeout_ms=1000)
```

## 6. 文件与打包

保持当前 `pycgraph` 扩展模块布局，不将其改造成 package，也不重命名已有模块：

```text
python/
├── PyCGraph.cpp
├── _pycgraph_message.py
├── setup.py
└── pyproject.toml
```

变更如下：

1. 新增 `python/_pycgraph_message.py`，包含全部纯 Python message 实现。
2. `setup.py` 增加 `py_modules=["_pycgraph_message"]`，确保 helper module 进入 wheel。
3. `PyCGraph.cpp` 保持 `PYBIND11_MODULE(pycgraph, cg)` 和扩展名 `pycgraph` 不变。
4. `PyCGraph.cpp` 在模块初始化末尾导入 `_pycgraph_message`，将公开类型和函数对象挂到 `pycgraph`。
5. 公开对象的 `__module__` 规范为 `pycgraph`，不向用户暴露内部 helper module 名称。

胶水层只在 `import pycgraph` 时转发一次 Python 对象引用。之后调用 `send_message()`、`recv_message()`、`pub_message()`、`sub_message()` 时直接执行纯 Python 函数，不发生逐消息的 Python -> C++ -> Python 转发。

安装后的结构为：

```text
site-packages/
├── pycgraph.cpython-<version>-<platform>.so
└── _pycgraph_message.py
```

现有使用方式保持不变：

```python
from pycgraph import GNode, GPipeline
```

新增 message API 使用同一入口：

```python
from pycgraph import create_message_topic, send_message
```

## 7. 内部架构

### 7.1 `_LocalMessageQueue`

`_LocalMessageQueue` 是职责单一的有界 SPSC FIFO，不公开给用户。

内部成员：

- `collections.deque`：保存 Python 对象引用。
- `capacity`：实际可保存的消息数量。
- 一个 `threading.Lock`。
- `not_empty` 和 `not_full` 两个共享该 Lock 的 `threading.Condition`。

只提供内部所需操作：

```python
push(message, strategy) -> int
pop(timeout_ms) -> object
```

不实现 `queue.Queue` 的 MPMC、`task_done()`、`join()` 等无关能力。

### 7.2 `_SendRecvTopic`

每个 send/recv topic 持有一个 `_LocalMessageQueue`。

约束：

- 一个 sender。
- 一个 receiver。
- runtime 不识别或校验调用者身份。
- 违反 SPSC 约束属于未定义使用方式。

### 7.3 `_PubSubTopic`

每个 pub/sub topic 持有：

- `conn_id -> _LocalMessageQueue` 映射。
- 一个缓存的 subscriber queue tuple。

bind/detach 时重建缓存 tuple。`pub_message()` 直接遍历该 tuple，避免每次 publish 构造临时 snapshot。

约束：

- 一个 publisher。
- 多个 subscriber。
- 每个 subscriber 拥有独立的有界队列和 capacity。
- runtime 不识别或校验 publisher 身份。

### 7.4 `_LocalMessageManager`

模块内只创建一个全局 manager，供同一 Python 解释器中的所有 node 和 pipeline 共享。

manager 持有：

- send/recv topic registry。
- pub/sub topic registry。
- `conn_id -> subscription` registry。
- 单调递增的 conn_id counter。
- 只用于生命周期操作的 registry lock。

send/recv 与 pub/sub 使用独立 registry，因此相同 topic 字符串可以同时存在于两种模式。

hot path 不获取 manager 全局锁。topic 创建、删除、bind、detach、drop、clear 等生命周期操作使用 registry lock。

## 8. 数据流与语义

### 8.1 Send/Recv

```text
Python sender
    -> manager 查找 topic
    -> topic queue.push(object reference)
    -> topic queue.pop()
    -> Python receiver
```

receiver 得到 sender 写入的同一个 Python 对象。由于一条消息只交给一个 receiver，receiver 可以修改该对象。

### 8.2 Pub/Sub

```text
Python publisher
    -> manager 查找 topic
    -> 遍历 subscriber queue tuple
    -> 每个 queue.push(same object reference)
    -> 各 subscriber 独立 queue.pop()
```

fan-out 只复制引用，不复制 payload，复杂度为 O(subscriber 数量)。

pub/sub 使用只读契约：

- publisher 调用 `pub_message()` 后不得修改已发布对象。
- subscriber 不得修改收到的对象。
- 需要修改时，由业务显式构造新对象或执行 copy。
- runtime 不包装代理、不执行浅拷贝或深拷贝，也不校验对象是否可变。

## 9. Push Strategy

### 9.1 `WAIT`

- 队列未满时直接写入。
- 队列已满时在 `not_full` 上等待。
- consumer 弹出消息后唤醒 producer。
- 等待期间释放 queue lock 和 GIL。

### 9.2 `REPLACE`

- 队列未满时直接写入。
- 队列已满时，在同一个 queue lock 临界区内移除最旧消息，再写入新消息。
- 整个替换过程是原子的，不使用非原子的 `get_nowait()`、`put_nowait()` 组合。

### 9.3 `DROP`

- 队列未满时直接写入并返回 `1`。
- 队列已满时不写入并返回 `0`。

### 9.4 Pub/Sub 的慢订阅者行为

每个 subscriber 独立应用 push strategy：

- `WAIT`：publisher 按 subscriber tuple 顺序写入。任一慢 subscriber 队列已满时，publisher 会阻塞，尚未遍历到的 subscriber 暂时收不到本次消息。
- `REPLACE`：只替换对应慢 subscriber 队列中的最旧消息。
- `DROP`：只在对应已满队列中丢弃，不影响其他 subscriber。

## 10. Timeout

`recv_message()` 与 `sub_message()` 使用相同规则：

- `timeout_ms=None`：一直等待。
- `timeout_ms=0`：非阻塞尝试。
- `timeout_ms>0`：最多等待指定毫秒数。
- `timeout_ms<0`：抛出 `GMessageError`。

有超时时间时，内部基于 monotonic deadline 循环检查队列，正确处理虚假唤醒。

超时抛出 `GMessageError`，不返回 `None` 或 sentinel。payload 不做特殊值限制。

## 11. 错误处理

所有 message 错误统一抛出 `GMessageError`。不定义 topic、connection、timeout 等细分异常类型。

错误原因通过稳定、可读的字符串提供，例如：

```text
message topic [topic] not found
message topic [topic] capacity mismatch, expect [64], actual [32]
message connection [3] not found
message connection [3] does not belong to topic [topic]
receive message timeout, topic [topic], timeout [1000] ms
```

公开 API 的非法 topic、capacity、timeout、conn_id 和 strategy 参数也统一转为 `GMessageError`。

## 12. Topic 生命周期

### 12.1 Send/Recv

- `create_message_topic()` 首次调用创建 topic。
- 相同 topic、相同 capacity 重复创建视为成功。
- 相同 topic、不同 capacity 抛出 `GMessageError`。
- `remove_message_topic()` 只删除 send/recv topic。

### 12.2 Pub/Sub

- `bind_message_topic()` 在 topic 下创建 subscriber queue，并返回新 conn_id。
- 同一 topic 的不同 subscriber 可以配置不同 capacity。
- `detach_message_subscription()` 同时删除 topic 下的 subscriber 和 conn_id registry 项。
- 最后一个 subscriber detach 后，空 topic 继续存在，`pub_message()` 返回 `0`；由 `drop_message_topic()` 显式删除 topic。
- `drop_message_topic()` 删除 topic、全部 subscriber queue 和对应 conn_id registry 项。

### 12.3 Clear 与 Conn ID

- `clear_messages()` 清空两类 topic、全部 subscriber 和 conn_id registry。
- conn_id counter 不重置，继续单调递增，避免旧 conn_id 在 clear 后错误指向新 subscription。

### 12.4 生命周期并发限制

topic 创建、删除、bind、detach、drop、clear 不允许与同一 topic 的 send/recv/pub/sub 并发。

该限制避免在每次消息收发中增加 manager 全局锁，也避免删除 topic 时存在无法安全唤醒的阻塞 waiter。

## 13. Runtime 注意事项

- Python node 由 CGraph worker thread 调用，但 message hot path 保持在 Python 层。
- `Condition.wait()` 会在阻塞期间释放 GIL，使另一个 Python worker 可以发送或接收。
- 阻塞 message 操作仍会占用对应的 CGraph worker thread。
- 若 thread pool 只有一个可用 worker，或 DAG 依赖关系要求被阻塞节点完成后对端才能运行，`WAIT` 可能造成业务死锁。
- message manager 不尝试检测调度层死锁；教程和文档需要说明该约束。

## 14. 测试

不增加 benchmark。测试使用标准库能力，避免引入新的 Python 测试运行时依赖。

### 14.1 Queue 测试

- FIFO 与 capacity。
- 对象 identity，确认没有 payload copy。
- `WAIT` 阻塞与唤醒。
- `DROP` 满队列行为和返回值。
- `REPLACE` 只替换最旧一条消息。
- `REPLACE` 与 consumer 并发时保持原子语义。
- 永久等待、非阻塞和有限超时。
- 虚假唤醒循环条件。

### 14.2 Manager 功能测试

- send/recv 完整流程。
- pub/sub 的 1 个和多个 subscriber。
- subscriber 使用不同 capacity。
- `pub_message()` 成功入队数量。
- 重复创建 topic。
- 同名 send/recv 与 pub/sub topic 并存。
- remove、bind、detach、drop、clear。
- clear 后 conn_id 保持单调递增。
- topic、conn_id、capacity、timeout 和 strategy 错误字符串。

线程测试使用 `Event`、`Barrier` 等同步原语控制时序，不依赖长时间 `sleep` 猜测执行顺序。

### 14.3 公开 API 与打包测试

- 从 `_pycgraph_message` 直接运行纯 Python 单元测试。
- 构建 wheel 后安装到干净环境。
- 验证现有 `GNode`、`GPipeline` 等 API 导入不变。
- 验证 message 类型和函数可以从 `pycgraph` 顶层导入。
- 验证公开类型和函数不暴露内部 helper module 名称。

### 14.4 教程

新增：

- `python/tutorial/T16-MessageSendRecv.py`
- `python/tutorial/T17-MessagePubSub.py`

编号和主题与 C++ tutorial 对应。教程覆盖正常路径、timeout 处理和资源清理。

## 15. 性能与 Trade-off

预期 hot path 成本包括：

- 一次 Python 函数调用。
- 一次 manager dict 查找。
- 一次 queue lock。
- 一次 deque append 或 popleft。
- 必要的 Python 引用计数变化。

不会发生：

- payload 序列化。
- payload 浅拷贝或深拷贝。
- 每条消息的 Python -> C++ -> Python 转发。
- 每条消息的 wrapper 或 result object 分配。

`pub_message()` 对每个 subscriber 必须进行一次入队和唤醒，O(N) fan-out 是语义要求带来的必要成本。

主要 trade-off：

- 自研 `_LocalMessageQueue` 增加少量维护代码，但获得严格、可解释的 `REPLACE` 原子语义，且不依赖 `queue.Queue` 私有字段。
- SPSC/SPMC 约束减少实现复杂度，但 runtime 不强制验证，误用时不保证行为。
- pub/sub 通过只读契约实现零 payload copy，但 Python 无法低成本强制该契约。
- 不做 benchmark，因此本次只描述成本模型，不声明未经测量的性能提升数字。

## 16. 完成标准

满足以下条件时，功能实现完成：

1. 现有 `pycgraph` 模块名、扩展名和已有导入方式不变。
2. 所有公开 message API 均可从 `pycgraph` 顶层导入。
3. send/recv、pub/sub 和三种 push strategy 行为符合本文档。
4. hot path 只传递对象引用，不序列化、不复制 payload、不进入 C++ message manager。
5. 只使用一个 `GMessageError` 报告 message 错误。
6. 生命周期、并发、打包和 tutorial 测试通过。
7. 实现不包含跨进程、asyncio、metrics 或 benchmark 等范围外能力。
