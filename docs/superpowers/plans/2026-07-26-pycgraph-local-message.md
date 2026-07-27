# PyCGraph Local Message Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use `superpowers:executing-plans` to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add pure-Python, single-process `send/recv` and `pub/sub` message support behind the sole public façade `pycgraph.GMessage`.

**Architecture:** A single flat `python/src/_pycgraph_message.py` module owns the bounded SPSC queue, send/recv and pub/sub topic records, the interpreter-global manager, and the public static façade. Queue locks protect message state; a manager registry lock protects lifecycle mutation only, so send/recv/pub/sub hot paths perform a dictionary lookup and per-queue synchronization without taking the global lock.

**Tech Stack:** Python 3.8+ standard library (`collections`, `enum`, `threading`, `time`), CPython/pybind11 extension glue, setuptools, `unittest`.

## Global Constraints

- Keep the extension module and `PYBIND11_MODULE(pycgraph, cg)` name unchanged.
- Put every message runtime `.py` file directly under `python/src/`.
- Export only `GMessage` from `pycgraph`; do not export message methods or internal helpers at module level.
- Pass Python object references only; do not serialize, copy, wrap, or enter the C++ message manager.
- Support only the documented SPSC send/recv and single-publisher/multi-subscriber contracts.
- Do not add IPC, `asyncio`, metrics, tracing, benchmarks, payload validation, or pipeline-destroy cleanup.
- Do not commit local changes; the user will review and commit them.

---

### Task 1: Bounded local queue

**Files:**
- Create: `python/tests/test_pycgraph_message.py`
- Create: `python/src/_pycgraph_message.py`
- Modify: `.gitignore`

**Interfaces:**
- Produces: `_LocalMessageQueue(capacity: int)`, `push(message: object, strategy: _PushStrategy) -> int`, and `pop(timeout_ms: Optional[int], timeout_error: str) -> object`.
- Produces: `_PushStrategy` values `WAIT=1`, `REPLACE=2`, and `DROP=3`.
- Produces: one `PyCGraphException` implementation type, publicly exposed as
  `GMessage.Error`.

- [x] **Step 1: Make `python/src/` trackable and write failing queue tests**

  Remove the obsolete `/python/src` ignore entry, add the test package, and cover FIFO, identity, full-queue `DROP`, oldest-item `REPLACE`, blocking `WAIT`, wakeup, `None`/zero/finite timeout, spurious wakeups, and the atomic replace critical section. Tests import `_pycgraph_message` by prepending `python/src/` to `sys.path`.

- [x] **Step 2: Verify the queue tests fail for the missing module**

  Run: `python3.13 -m unittest python/tests/test_pycgraph_message.py -v`

  Expected: import failure for `_pycgraph_message`.

- [x] **Step 3: Implement the minimal bounded queue**

  Use one `deque`, one `Lock`, and `not_empty`/`not_full` conditions sharing
  that lock. `WAIT` loops while full, `REPLACE` performs `popleft()` and
  `append()` in one lock scope, and `DROP` returns `0` without mutation.
  Timed pop uses `time.monotonic()` and a remaining-time loop, then directly
  raises `PyCGraphException` with the supplied contextual description.

- [x] **Step 4: Verify queue behavior**

  Run: `python3.13 -m unittest python.tests.test_pycgraph_message.LocalMessageQueueTest -v`

  Expected: all queue tests pass with no errors or warnings.

### Task 2: Manager lifecycle and message semantics

**Files:**
- Modify: `python/tests/test_pycgraph_message.py`
- Modify: `python/src/_pycgraph_message.py`

**Interfaces:**
- Produces: `_LocalMessageManager` methods matching the nine public lifecycle/send/receive operations.
- Produces: `_SendRecvTopic(queue)` and `_PubSubTopic(subscribers, subscriber_queues)` records.
- Produces: an interpreter-global `_MANAGER`; its connection counter starts at `1` and is never reset by `clear_messages()`.

- [x] **Step 1: Write failing manager tests**

  Cover send/recv identity; one and multiple subscribers; per-subscriber capacity; publish success counts for all strategies; duplicate topic creation; capacity mismatch; coexistence of same-name send/recv and pub/sub topics; remove, bind, detach, drop, and clear; persistent empty pub/sub topics; monotonic connection IDs after clear; and all documented not-found, mismatch, and timeout errors.

- [x] **Step 2: Verify manager tests fail for missing behavior**

  Run: `python3.13 -m unittest python.tests.test_pycgraph_message.LocalMessageManagerTest -v`

  Expected: failures because the manager operations do not yet exist.

- [x] **Step 3: Implement manager registries and topic records**

  Lifecycle methods take `_registry_lock`. Hot-path methods read the appropriate registry without that lock. Bind/detach rebuild `_PubSubTopic.subscriber_queues` as a tuple. Publish iterates that cached tuple in order and sums successful queue pushes.

- [x] **Step 4: Verify manager semantics**

  Run: `python3.13 -m unittest python.tests.test_pycgraph_message.LocalMessageManagerTest -v`

  Expected: all manager tests pass.

### Task 3: Public `GMessage` façade and validation

**Files:**
- Modify: `python/tests/test_pycgraph_message.py`
- Modify: `python/src/_pycgraph_message.py`

**Interfaces:**
- Produces: `GMessage.PushStrategy`, `GMessage.Error`, and these static methods:
  `create_message_topic`, `remove_message_topic`, `send_message`, `recv_message`,
  `bind_message_topic`, `pub_message`, `sub_message`,
  `detach_message_subscription`, `drop_message_topic`, and `clear_messages`.
- Enforces: topic is `str`; capacity is a non-boolean `int >= 1`; timeout is `None` or a non-boolean `int >= 0`; connection ID is a non-boolean `int > 0`; strategy is a `GMessage.PushStrategy` member.

- [x] **Step 1: Write failing façade tests**

  Test static invocation, exact enum values, a single `GMessage.Error` type for operational and argument failures, rejection of booleans as integer parameters, arbitrary payload acceptance including `None`, and `__module__ == "pycgraph"` for the façade and both nested public types.

- [x] **Step 2: Verify façade tests fail**

  Run: `python3.13 -m unittest python.tests.test_pycgraph_message.GMessageFacadeTest -v`

  Expected: failures because `GMessage` is not implemented.

- [x] **Step 3: Implement validation and façade forwarding**

  Define one internal enum and one internal runtime-error class, attach them as `GMessage.PushStrategy` and `GMessage.Error`, normalize their public metadata to `pycgraph`, and expose only static methods that validate arguments before forwarding to `_MANAGER`.

- [x] **Step 4: Verify all pure-Python tests**

  Run: `python3.13 -m unittest discover -s python/tests -v`

  Expected: all queue, manager, and façade tests pass.

### Task 4: Extension glue and wheel packaging

**Files:**
- Modify: `python/PyCGraph.cpp`
- Modify: `python/setup.py`
- Modify: `python/pyproject.toml`
- Create: `python/tests/test_pycgraph_integration.py`

**Interfaces:**
- `PyCGraph.cpp` imports `_pycgraph_message` once at module initialization and assigns only its `GMessage` attribute to `pycgraph`.
- Setuptools treats `python/src/` as the source root and packages the flat `_pycgraph_message` module alongside the `pycgraph` extension.

- [x] **Step 1: Write the failing installed-module integration test**

  Assert existing `GNode` and `GPipeline` imports, top-level `GMessage`, absence of top-level message functions, public `__module__` metadata, and a real send/recv plus pub/sub flow through the installed extension.

- [x] **Step 2: Verify the current extension lacks `GMessage`**

  Build/install the pre-glue wheel in a temporary environment and run the integration test.

  Expected: failure importing `GMessage` from `pycgraph`.

- [x] **Step 3: Add minimal pybind11 glue and packaging configuration**

  Append one `_pycgraph_message` import and one `cg.attr("GMessage")` assignment at the end of module initialization. Configure `package_dir={"": "src"}` and `py_modules=["_pycgraph_message"]` consistently in setuptools metadata.

- [x] **Step 4: Build and inspect the wheel**

  Run from `python/`: `python3.13 -m pip wheel . --no-build-isolation --no-deps --wheel-dir /tmp/pycgraph-message-wheel`

  Inspect the archive and require both the platform `pycgraph` extension and `_pycgraph_message.py`, with no nested message source package.

- [x] **Step 5: Install into a clean temporary virtual environment and verify**

  Install the wheel with `--no-deps`, then run `python/tests/test_pycgraph_integration.py` using that interpreter.

  Expected: all integration tests pass, and existing imports remain available.

### Task 5: Tutorials and final verification

**Files:**
- Create: `python/tutorial/T16-MessageSendRecv.py`
- Create: `python/tutorial/T17-MessagePubSub.py`
- Create: `python/tutorial/MyParams/MyMessageParam.py`
- Create: `python/tutorial/MyGNode/MySendMessageNode.py`
- Create: `python/tutorial/MyGNode/MyRecvMessageNode.py`
- Create: `python/tutorial/MyGNode/MyPubMessageNode.py`
- Create: `python/tutorial/MyGNode/MySubMessageNode.py`
- Create: `python/tests/test_message_tutorials.py`

**Interfaces:**
- Message operations execute inside `GNode.run()` callbacks managed by real
  `GPipeline` instances.
- T16 runs separate sender and receiver pipelines concurrently for three
  iterations.
- T17 runs one publisher and three subscriber pipelines concurrently for five
  iterations.
- Each tutorial demonstrates the normal path, catches `GMessage.Error` for a finite timeout, and cleans up resources in `finally`.

- [x] **Step 1: Add the two focused tutorials**

  Mirror the C++ tutorial topology, node dependencies, topic capacities,
  iteration counts, payload values, and publisher initialization delay. Both
  tutorials also explain that blocking message operations occupy a worker
  thread and can deadlock an undersized or dependency-constrained pipeline.

- [x] **Step 2: Run tutorials against the clean wheel environment**

  Run: `/tmp/pycgraph-message-venv/bin/python python/tutorial/T16-MessageSendRecv.py`

  Run: `/tmp/pycgraph-message-venv/bin/python python/tutorial/T17-MessagePubSub.py`

  Expected: both exit `0`, show normal delivery and the handled timeout, and leave no message resources behind.

- [x] **Step 3: Run fresh complete verification**

  Run pure-Python unit tests, installed-extension integration tests, both tutorials, and a wheel-content inspection in one fresh pass. Then inspect `git diff --check`, `git status --short`, and the complete diff against every requirement in the design spec.

  Expected: all tests and tutorials pass, the wheel is complete, no whitespace errors exist, and no out-of-scope feature or C++ message-path change is present.
