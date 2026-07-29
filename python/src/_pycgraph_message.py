"""
Single-process message support for pycgraph.

The public entry point is ``pycgraph.GMessagePy``. Everything else in this
module is an implementation detail.
"""

from typing import Optional

from _pycgraph_message_manager import _LocalMessageManager
from _pycgraph_message_primitives import (
    _LocalMessageQueue,
    _PushStrategy,
    PyCGraphException,
)


_MANAGER = _LocalMessageManager()


def _validate_topic(topic):
    if not isinstance(topic, str):
        raise PyCGraphException("message topic must be str")


def _validate_capacity(capacity):
    if type(capacity) is not int or capacity < 1:
        raise PyCGraphException(
            "message capacity must be an int greater than or equal to 1"
        )


def _validate_timeout(timeout_ms):
    if timeout_ms is not None and (
        type(timeout_ms) is not int or timeout_ms < 0
    ):
        raise PyCGraphException(
            "message timeout must be None or an int "
            "greater than or equal to 0"
        )


def _validate_conn_id(conn_id):
    if type(conn_id) is not int or conn_id <= 0:
        raise PyCGraphException(
            "message connection must be an int greater than 0"
        )


def _validate_strategy(strategy):
    if not isinstance(strategy, _PushStrategy):
        raise PyCGraphException(
            "message push strategy must be GMessagePy.PushStrategy"
        )


class GMessagePy:
    """Static façade for single-process Python message operations."""

    PushStrategy = _PushStrategy
    Error = PyCGraphException

    @staticmethod
    def create_message_topic(topic: str, capacity: int) -> None:
        _validate_topic(topic)
        _validate_capacity(capacity)
        _MANAGER.create_message_topic(topic, capacity)

    @staticmethod
    def remove_message_topic(topic: str) -> None:
        _validate_topic(topic)
        _MANAGER.remove_message_topic(topic)

    @staticmethod
    def send_message(
        topic: str,
        message: object,
        strategy: _PushStrategy = _PushStrategy.WAIT,
    ) -> int:
        _validate_topic(topic)
        _validate_strategy(strategy)
        return _MANAGER.send_message(topic, message, strategy)

    @staticmethod
    def recv_message(
        topic: str,
        timeout_ms: Optional[int] = None,
    ) -> object:
        _validate_topic(topic)
        _validate_timeout(timeout_ms)
        return _MANAGER.recv_message(topic, timeout_ms)

    @staticmethod
    def bind_message_topic(topic: str, capacity: int) -> int:
        _validate_topic(topic)
        _validate_capacity(capacity)
        return _MANAGER.bind_message_topic(topic, capacity)

    @staticmethod
    def pub_message(
        topic: str,
        message: object,
        strategy: _PushStrategy = _PushStrategy.WAIT,
    ) -> int:
        _validate_topic(topic)
        _validate_strategy(strategy)
        return _MANAGER.pub_message(topic, message, strategy)

    @staticmethod
    def sub_message(
        conn_id: int,
        timeout_ms: Optional[int] = None,
    ) -> object:
        _validate_conn_id(conn_id)
        _validate_timeout(timeout_ms)
        return _MANAGER.sub_message(conn_id, timeout_ms)

    @staticmethod
    def detach_message_subscription(topic: str, conn_id: int) -> None:
        _validate_topic(topic)
        _validate_conn_id(conn_id)
        _MANAGER.detach_message_subscription(topic, conn_id)

    @staticmethod
    def drop_message_topic(topic: str) -> None:
        _validate_topic(topic)
        _MANAGER.drop_message_topic(topic)

    @staticmethod
    def clear_messages() -> None:
        _MANAGER.clear_messages()


GMessagePy.__module__ = "pycgraph"
_PushStrategy.__name__ = "PushStrategy"
_PushStrategy.__qualname__ = "GMessagePy.PushStrategy"
_PushStrategy.__module__ = "pycgraph"
PyCGraphException.__name__ = "Error"
PyCGraphException.__qualname__ = "GMessagePy.Error"
PyCGraphException.__module__ = "pycgraph"
