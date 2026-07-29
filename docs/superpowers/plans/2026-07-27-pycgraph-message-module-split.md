# PyCGraph Message Module Split Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Split the pure-Python message runtime into three responsibility-focused flat modules without changing public behavior, hot-path behavior, or the `pycgraph.GMessagePy` API.

**Architecture:** `_pycgraph_message_primitives.py` owns the strategy, exception, and bounded queue; `_pycgraph_message_manager.py` owns topic records and registry/lifecycle behavior; `_pycgraph_message.py` remains the sole extension-facing entry and owns validation, the interpreter-global manager, and the public façade. Dependencies are one-way: primitives → manager → entry, with no runtime import cycles.

**Tech Stack:** Python 3.8+ standard library, CPython/pybind11 extension glue, setuptools, `unittest`.

## Global Constraints

- Keep `PYBIND11_MODULE(pycgraph, cg)`, the extension filename, and `from pycgraph import GMessagePy` unchanged.
- Keep every message runtime `.py` file directly under `python/src/`.
- Use `_pycgraph_message_*` names because these helpers are installed as top-level flat modules.
- Export only `GMessagePy` from the `pycgraph` extension.
- Pass Python object references directly; do not serialize, copy, wrap, or enter the C++ message manager.
- Preserve the existing queue locking, timeout, registry locking, and cached subscriber tuple behavior exactly.
- Do not add dependencies, package nesting, metrics, tracing, or unrelated refactoring.
- Preserve the user's existing uncommitted changes and do not commit implementation changes.

---

### Task 1: Lock the internal module boundaries with a failing test

**Files:**
- Modify: `python/tests/test_pycgraph_message.py`

**Interfaces:**
- Consumes: the existing `_PushStrategy`, `PyCGraphException`, `_LocalMessageQueue`, and `_LocalMessageManager` implementation types.
- Produces: a regression test requiring the new modules to own those exact types while `_pycgraph_message` remains the façade entry.

- [x] **Step 1: Add a module ownership test**

Add `import importlib` with the standard-library imports and add:

```python
class MessageModuleLayoutTest(unittest.TestCase):
    def test_internal_types_are_owned_by_focused_modules(self):
        entry = importlib.import_module("_pycgraph_message")
        primitives = importlib.import_module(
            "_pycgraph_message_primitives"
        )
        manager = importlib.import_module("_pycgraph_message_manager")

        self.assertIs(primitives._PushStrategy, entry._PushStrategy)
        self.assertIs(
            primitives.PyCGraphException,
            entry.PyCGraphException,
        )
        self.assertIs(primitives._LocalMessageQueue, entry._LocalMessageQueue)
        self.assertIs(
            manager._LocalMessageManager,
            entry._LocalMessageManager,
        )
        self.assertEqual(
            "_pycgraph_message_primitives",
            primitives._LocalMessageQueue.__module__,
        )
        self.assertEqual(
            "_pycgraph_message_manager",
            manager._LocalMessageManager.__module__,
        )
```

- [x] **Step 2: Run the focused test and confirm the RED state**

Run:

```bash
python3.13 -m unittest \
  python.tests.test_pycgraph_message.MessageModuleLayoutTest -v
```

Expected: FAIL because `_pycgraph_message_primitives` does not exist.

---

### Task 2: Split implementation into a one-way module graph

**Files:**
- Create: `python/src/_pycgraph_message_primitives.py`
- Create: `python/src/_pycgraph_message_manager.py`
- Modify: `python/src/_pycgraph_message.py`
- Test: `python/tests/test_pycgraph_message.py`

**Interfaces:**
- Produces from primitives: `_PushStrategy`, `PyCGraphException`, and `_LocalMessageQueue(capacity)` with unchanged `push(message, strategy)` and `pop(timeout_ms, timeout_error)` behavior.
- Produces from manager: `_SendRecvTopic`, `_PubSubTopic`, and `_LocalMessageManager` with the existing ten manager operations.
- Produces from entry: the same `GMessagePy`, `_MANAGER`, validation functions, and compatibility bindings for the four internal types imported by existing tests.

- [x] **Step 1: Create the primitives module**

Move the definitions of `_PushStrategy`, `PyCGraphException`, and
`_LocalMessageQueue` unchanged into `_pycgraph_message_primitives.py`.
Its imports are exactly:

```python
from collections import deque
from enum import IntEnum
import threading
import time
```

- [x] **Step 2: Create the manager module**

Move `_SendRecvTopic`, `_PubSubTopic`, and `_LocalMessageManager` unchanged
into `_pycgraph_message_manager.py`. Import their dependencies with:

```python
import threading

from _pycgraph_message_primitives import (
    _LocalMessageQueue,
    PyCGraphException,
)
```

- [x] **Step 3: Reduce the entry module to façade responsibilities**

Keep validation functions, `_MANAGER`, `GMessagePy`, and public metadata
normalization in `_pycgraph_message.py`. Replace the removed implementation
with:

```python
from typing import Optional

from _pycgraph_message_manager import _LocalMessageManager
from _pycgraph_message_primitives import (
    _LocalMessageQueue,
    _PushStrategy,
    PyCGraphException,
)
```

Construct `_MANAGER = _LocalMessageManager()` in the entry module exactly
once. Retain `_LocalMessageQueue` and `_LocalMessageManager` as imported
entry attributes so current internal unit-test imports remain compatible.

- [x] **Step 4: Run the focused test and confirm the GREEN state**

Run:

```bash
python3.13 -m unittest \
  python.tests.test_pycgraph_message.MessageModuleLayoutTest -v
```

Expected: PASS.

- [x] **Step 5: Run all pure-Python message tests**

Run:

```bash
python3.13 -m unittest python.tests.test_pycgraph_message -v
```

Expected: all tests pass with no deadlock, timeout, or identity regression.

---

### Task 3: Package every flat helper module

**Files:**
- Modify: `python/setup.py`
- Modify: `python/pyproject.toml`
- Test: `python/tests/test_pycgraph_integration.py`

**Interfaces:**
- Consumes: the three top-level modules under `python/src/`.
- Produces: a wheel containing the extension and all three Python runtime modules.

- [x] **Step 1: Update both setuptools module declarations**

Set the same module list in `setup.py` and `pyproject.toml`:

```python
[
    "_pycgraph_message",
    "_pycgraph_message_primitives",
    "_pycgraph_message_manager",
]
```

Use valid TOML list syntax in `pyproject.toml`.

- [x] **Step 2: Build a wheel**

Run from `python/`:

```bash
python3.13 -m pip wheel . \
  --no-build-isolation \
  --no-deps \
  --wheel-dir /tmp/pycgraph-message-module-split-wheel
```

Expected: one `pycgraph-3.2.4-*.whl` is produced.

- [x] **Step 3: Inspect the wheel module list**

Run:

```bash
python3.13 -m zipfile -l \
  /tmp/pycgraph-message-module-split-wheel/pycgraph-3.2.4-*.whl
```

Expected: the archive contains:

```text
_pycgraph_message.py
_pycgraph_message_primitives.py
_pycgraph_message_manager.py
pycgraph.cpython-*-*.so
```

- [x] **Step 4: Run clean-wheel integration tests**

Create a temporary virtual environment, install the built wheel with
`--no-deps`, and execute:

```bash
python/tests/test_pycgraph_integration.py
```

Expected: all existing imports, send/recv, pub/sub, and public metadata tests pass.

---

### Task 4: Final regression and diff audit

**Files:**
- Verify: `python/src/_pycgraph_message.py`
- Verify: `python/src/_pycgraph_message_primitives.py`
- Verify: `python/src/_pycgraph_message_manager.py`
- Verify: `python/tests/`
- Verify: `python/tutorial/`

**Interfaces:**
- Consumes: the packaged three-module runtime.
- Produces: evidence that behavior and tutorials remain unchanged.

- [x] **Step 1: Run the complete Python test suite**

Run:

```bash
python3.13 -m unittest discover -s python/tests -v
```

Expected: all tests pass.

- [x] **Step 2: Audit imports and module declarations**

Run:

```bash
rg -n "_pycgraph_message" \
  python/src python/tests python/setup.py python/pyproject.toml python/PyCGraph.cpp
```

Expected: the extension imports only `_pycgraph_message`; manager imports only
primitives; entry imports manager and primitives; both packaging files list all
three modules.

- [x] **Step 3: Review the final diff**

Run:

```bash
git diff --check
git status --short
git diff -- python/src python/tests python/setup.py python/pyproject.toml
```

Expected: no whitespace errors, no unrelated edits, and no behavioral changes
outside module ownership and packaging.
