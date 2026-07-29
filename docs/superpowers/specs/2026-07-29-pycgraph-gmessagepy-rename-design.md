# PyCGraph `GMessagePy` Rename Design

**Date:** 2026-07-29

## Goal

Rename the pure-Python message API from `GMessage` to `GMessagePy` so that
its runtime implementation is explicit and a future C++-backed Python API can
use a distinct name such as `GMessageCpp`.

## Public API

- Replace `pycgraph.GMessage` with `pycgraph.GMessagePy`.
- Do not keep a `GMessage` compatibility alias.
- Preserve all message methods, arguments, return values, exception behavior,
  queue semantics, and runtime performance characteristics.
- Rename nested public metadata consistently:
  - `GMessagePy.PushStrategy`
  - `GMessagePy.Error`

## Python Source and Packaging

- Keep the internal entry module named `_pycgraph_message.py`; only its public
  façade class changes from `GMessage` to `GMessagePy`.
- Keep `_pycgraph_message_primitives` and `_pycgraph_message_manager`
  unchanged because they describe implementation responsibilities rather than
  a public backend name.
- Keep the pybind11 import of `_pycgraph_message` and export only
  `GMessagePy`.
- Keep both setuptools module lists unchanged and verify that
  `_pycgraph_message` remains included in source distributions and wheels.

## Tests and Tutorials

- Change unit and integration tests to import and exercise `GMessagePy`.
- Add an integration assertion that `pycgraph.GMessage` is absent, preventing
  an accidental compatibility alias.
- Update all Python tutorials and tutorial helper nodes to use `GMessagePy`.
- Update existing message design and implementation-plan documents so their
  recorded API matches the codebase.

## Compatibility and Runtime Impact

This is an intentional source-incompatible API rename. Existing callers must
replace `GMessage` with `GMessagePy`.

The change adds no runtime abstraction, backend selection, allocation, lock,
copy, or hot-path branch. Module import and attribute forwarding remain
one-time initialization work; message send/receive and publish/subscribe paths
are unchanged.

## Verification

- First change the tests and confirm they fail because `GMessagePy` is not yet
  available.
- Apply the production export rename and validate the unchanged packaging
  declarations.
- Run message unit tests, extension integration tests, and tutorial tests.
- Build/install the package through its normal test path to verify that
  `_pycgraph_message` remains packaged correctly and exports `GMessagePy`.
- Search the Python source, tests, tutorials, and active design/plan documents
  for stale standalone `GMessage` references.
