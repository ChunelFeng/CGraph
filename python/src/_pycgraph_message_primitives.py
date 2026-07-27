"""
Low-level primitives for the pycgraph local message runtime.
"""

from collections import deque
from enum import IntEnum
import threading
import time


class _PushStrategy(IntEnum):
    WAIT = 1
    REPLACE = 2
    DROP = 3


class PyCGraphException(RuntimeError):
    """The single exception type used by the Python message runtime."""

    def __init__(self, info):
        self.info = info
        super().__init__(info)


class _LocalMessageQueue:
    """A bounded SPSC FIFO storing Python object references."""

    def __init__(self, capacity):
        self.capacity = capacity
        self._messages = deque()
        self._lock = threading.Lock()
        self._not_empty = threading.Condition(self._lock)
        self._not_full = threading.Condition(self._lock)

    def push(self, message, strategy):
        with self._lock:
            if strategy == _PushStrategy.WAIT:
                while len(self._messages) >= self.capacity:
                    self._not_full.wait()
            elif strategy == _PushStrategy.REPLACE:
                if len(self._messages) >= self.capacity:
                    self._messages.popleft()
            elif len(self._messages) >= self.capacity:
                return 0

            self._messages.append(message)
            self._not_empty.notify()
            return 1

    def pop(self, timeout_ms, timeout_error="message queue timeout"):
        with self._lock:
            if timeout_ms is None:
                while not self._messages:
                    self._not_empty.wait()
            else:
                deadline = time.monotonic() + timeout_ms / 1000.0
                while not self._messages:
                    remaining = deadline - time.monotonic()
                    if remaining <= 0:
                        raise PyCGraphException(timeout_error)
                    self._not_empty.wait(remaining)

            message = self._messages.popleft()
            self._not_full.notify()
            return message
