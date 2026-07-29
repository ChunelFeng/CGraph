"""
Tests for the pure-Python local message runtime.
"""

import importlib
import pathlib
import sys
import threading
import time
import unittest


PYTHON_ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PYTHON_ROOT / "src"))

from _pycgraph_message import (  # noqa: E402
    GMessagePy,
    _LocalMessageQueue,
    _LocalMessageManager,
    PyCGraphException,
    _PushStrategy,
)


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


class LocalMessageQueueTest(unittest.TestCase):
    def test_fifo_preserves_object_identity_up_to_capacity(self):
        queue = _LocalMessageQueue(capacity=2)
        first = object()
        second = object()

        self.assertEqual(1, queue.push(first, _PushStrategy.WAIT))
        self.assertEqual(1, queue.push(second, _PushStrategy.WAIT))

        self.assertIs(first, queue.pop(timeout_ms=0))
        self.assertIs(second, queue.pop(timeout_ms=0))

    def test_drop_returns_zero_and_preserves_queued_message_when_full(self):
        queue = _LocalMessageQueue(capacity=1)
        first = object()

        self.assertEqual(1, queue.push(first, _PushStrategy.DROP))
        self.assertEqual(0, queue.push(object(), _PushStrategy.DROP))
        self.assertIs(first, queue.pop(timeout_ms=0))

    def test_replace_removes_only_the_oldest_message(self):
        queue = _LocalMessageQueue(capacity=2)
        first = object()
        second = object()
        third = object()
        queue.push(first, _PushStrategy.WAIT)
        queue.push(second, _PushStrategy.WAIT)

        self.assertEqual(1, queue.push(third, _PushStrategy.REPLACE))

        self.assertIs(second, queue.pop(timeout_ms=0))
        self.assertIs(third, queue.pop(timeout_ms=0))

    def test_wait_blocks_until_consumer_frees_capacity(self):
        queue = _LocalMessageQueue(capacity=1)
        first = object()
        second = object()
        queue.push(first, _PushStrategy.WAIT)
        entered_push = threading.Event()
        push_finished = threading.Event()

        def push_second():
            entered_push.set()
            queue.push(second, _PushStrategy.WAIT)
            push_finished.set()

        producer = threading.Thread(target=push_second)
        producer.start()
        self.assertTrue(entered_push.wait(timeout=1))
        self.assertFalse(push_finished.wait(timeout=0.05))

        self.assertIs(first, queue.pop(timeout_ms=0))
        self.assertTrue(push_finished.wait(timeout=1))
        self.assertIs(second, queue.pop(timeout_ms=0))
        producer.join(timeout=1)
        self.assertFalse(producer.is_alive())

    def test_pop_without_timeout_waits_until_message_arrives(self):
        queue = _LocalMessageQueue(capacity=1)
        message = object()
        entered_pop = threading.Event()
        pop_finished = threading.Event()
        result = []

        def pop_message():
            entered_pop.set()
            result.append(queue.pop(timeout_ms=None))
            pop_finished.set()

        consumer = threading.Thread(target=pop_message)
        consumer.start()
        self.assertTrue(entered_pop.wait(timeout=1))
        self.assertFalse(pop_finished.wait(timeout=0.05))

        queue.push(message, _PushStrategy.WAIT)

        self.assertTrue(pop_finished.wait(timeout=1))
        self.assertEqual([message], result)
        self.assertIs(message, result[0])
        consumer.join(timeout=1)
        self.assertFalse(consumer.is_alive())

    def test_zero_timeout_is_non_blocking(self):
        queue = _LocalMessageQueue(capacity=1)

        with self.assertRaises(PyCGraphException) as context:
            queue.pop(timeout_ms=0)

        self.assertEqual("message queue timeout", context.exception.info)
        self.assertEqual("message queue timeout", str(context.exception))

    def test_finite_timeout_uses_deadline(self):
        queue = _LocalMessageQueue(capacity=1)
        start = time.monotonic()

        with self.assertRaises(PyCGraphException):
            queue.pop(timeout_ms=30)

        self.assertGreaterEqual(time.monotonic() - start, 0.02)

    def test_spurious_notifications_do_not_end_timed_wait_early(self):
        queue = _LocalMessageQueue(capacity=1)
        entered_pop = threading.Event()
        elapsed = []

        def pop_message():
            entered_pop.set()
            start = time.monotonic()
            with self.assertRaises(PyCGraphException):
                queue.pop(timeout_ms=80)
            elapsed.append(time.monotonic() - start)

        consumer = threading.Thread(target=pop_message)
        consumer.start()
        self.assertTrue(entered_pop.wait(timeout=1))

        for _ in range(5):
            with queue._not_empty:
                queue._not_empty.notify()
            threading.Event().wait(0.005)

        consumer.join(timeout=1)
        self.assertFalse(consumer.is_alive())
        self.assertEqual(1, len(elapsed))
        self.assertGreaterEqual(elapsed[0], 0.06)

    def test_replace_keeps_remove_and_append_in_one_critical_section(self):
        queue = _LocalMessageQueue(capacity=1)
        removed = threading.Event()
        release_finalizer = threading.Event()
        replacement = object()

        class BlockingFinalizer:
            def __del__(self):
                removed.set()
                release_finalizer.wait(timeout=1)

        queue.push(BlockingFinalizer(), _PushStrategy.WAIT)
        producer = threading.Thread(
            target=lambda: queue.push(replacement, _PushStrategy.REPLACE)
        )
        producer.start()
        self.assertTrue(removed.wait(timeout=1))

        consumer_entered = threading.Event()
        result = []
        errors = []

        def pop_replacement():
            consumer_entered.set()
            try:
                result.append(queue.pop(timeout_ms=0))
            except PyCGraphException as error:
                errors.append(error)

        consumer = threading.Thread(target=pop_replacement)
        consumer.start()
        self.assertTrue(consumer_entered.wait(timeout=1))
        release_finalizer.set()

        producer.join(timeout=1)
        consumer.join(timeout=1)
        self.assertFalse(producer.is_alive())
        self.assertFalse(consumer.is_alive())
        self.assertEqual([], errors)
        self.assertEqual(1, len(result))
        self.assertIs(replacement, result[0])


class LocalMessageManagerTest(unittest.TestCase):
    def setUp(self):
        self.manager = _LocalMessageManager()

    def test_send_recv_preserves_identity(self):
        message = object()
        self.manager.create_message_topic("send-recv", capacity=2)

        self.assertEqual(
            1,
            self.manager.send_message(
                "send-recv", message, _PushStrategy.WAIT
            ),
        )
        self.assertIs(
            message,
            self.manager.recv_message("send-recv", timeout_ms=0),
        )

    def test_duplicate_send_recv_topic_requires_matching_capacity(self):
        self.manager.create_message_topic("topic", capacity=2)

        self.manager.create_message_topic("topic", capacity=2)

        with self.assertRaisesRegex(
            PyCGraphException,
            r"^message topic \[topic\] capacity mismatch, "
            r"existing \[2\], requested \[3\]$",
        ):
            self.manager.create_message_topic("topic", capacity=3)

    def test_remove_deletes_only_send_recv_topic(self):
        self.manager.create_message_topic("shared", capacity=1)
        conn_id = self.manager.bind_message_topic("shared", capacity=1)

        self.manager.remove_message_topic("shared")

        with self.assertRaisesRegex(
            PyCGraphException, r"^message topic \[shared\] not found$"
        ):
            self.manager.send_message(
                "shared", object(), _PushStrategy.WAIT
            )
        self.assertEqual(
            1,
            self.manager.pub_message(
                "shared", object(), _PushStrategy.WAIT
            ),
        )
        self.manager.sub_message(conn_id, timeout_ms=0)

    def test_pub_sub_fans_out_same_reference(self):
        first_conn = self.manager.bind_message_topic("pub-sub", capacity=2)
        second_conn = self.manager.bind_message_topic(
            "pub-sub", capacity=3
        )
        message = object()

        self.assertEqual(
            2,
            self.manager.pub_message(
                "pub-sub", message, _PushStrategy.WAIT
            ),
        )

        self.assertIs(
            message, self.manager.sub_message(first_conn, timeout_ms=0)
        )
        self.assertIs(
            message, self.manager.sub_message(second_conn, timeout_ms=0)
        )

    def test_pub_drop_counts_only_subscribers_with_capacity(self):
        capacity_one = self.manager.bind_message_topic("topic", capacity=1)
        capacity_two = self.manager.bind_message_topic("topic", capacity=2)
        first = object()
        second = object()

        self.assertEqual(
            2,
            self.manager.pub_message(
                "topic", first, _PushStrategy.DROP
            ),
        )
        self.assertEqual(
            1,
            self.manager.pub_message(
                "topic", second, _PushStrategy.DROP
            ),
        )
        self.assertEqual(
            0,
            self.manager.pub_message(
                "topic", object(), _PushStrategy.DROP
            ),
        )

        self.assertIs(
            first, self.manager.sub_message(capacity_one, timeout_ms=0)
        )
        self.assertIs(
            first, self.manager.sub_message(capacity_two, timeout_ms=0)
        )
        self.assertIs(
            second, self.manager.sub_message(capacity_two, timeout_ms=0)
        )

    def test_pub_replace_updates_each_full_subscriber(self):
        first_conn = self.manager.bind_message_topic("topic", capacity=1)
        second_conn = self.manager.bind_message_topic("topic", capacity=1)
        replacement = object()
        self.manager.pub_message("topic", object(), _PushStrategy.WAIT)

        self.assertEqual(
            2,
            self.manager.pub_message(
                "topic", replacement, _PushStrategy.REPLACE
            ),
        )

        self.assertIs(
            replacement,
            self.manager.sub_message(first_conn, timeout_ms=0),
        )
        self.assertIs(
            replacement,
            self.manager.sub_message(second_conn, timeout_ms=0),
        )

    def test_detach_leaves_empty_topic_until_drop(self):
        conn_id = self.manager.bind_message_topic("topic", capacity=1)

        self.manager.detach_message_subscription("topic", conn_id)

        self.assertEqual(
            0,
            self.manager.pub_message(
                "topic", object(), _PushStrategy.WAIT
            ),
        )
        with self.assertRaisesRegex(
            PyCGraphException,
            rf"^message connection \[{conn_id}\] not found$",
        ):
            self.manager.sub_message(conn_id, timeout_ms=0)

        self.manager.drop_message_topic("topic")
        with self.assertRaisesRegex(
            PyCGraphException, r"^message topic \[topic\] not found$"
        ):
            self.manager.pub_message(
                "topic", object(), _PushStrategy.WAIT
            )

    def test_detach_rejects_connection_from_another_topic(self):
        conn_id = self.manager.bind_message_topic("first", capacity=1)
        self.manager.bind_message_topic("second", capacity=1)

        with self.assertRaisesRegex(
            PyCGraphException,
            rf"^message connection \[{conn_id}\] "
            r"does not belong to topic \[second\]$",
        ):
            self.manager.detach_message_subscription("second", conn_id)

    def test_drop_removes_all_topic_connections(self):
        first_conn = self.manager.bind_message_topic("topic", capacity=1)
        second_conn = self.manager.bind_message_topic("topic", capacity=1)

        self.manager.drop_message_topic("topic")

        for conn_id in (first_conn, second_conn):
            with self.assertRaisesRegex(
                PyCGraphException,
                rf"^message connection \[{conn_id}\] not found$",
            ):
                self.manager.sub_message(conn_id, timeout_ms=0)

    def test_clear_removes_both_topic_modes_without_reusing_conn_id(self):
        self.manager.create_message_topic("send-recv", capacity=1)
        old_conn_id = self.manager.bind_message_topic(
            "pub-sub", capacity=1
        )

        self.manager.clear_messages()

        with self.assertRaisesRegex(
            PyCGraphException, r"^message topic \[send-recv\] not found$"
        ):
            self.manager.recv_message("send-recv", timeout_ms=0)
        with self.assertRaisesRegex(
            PyCGraphException, r"^message topic \[pub-sub\] not found$"
        ):
            self.manager.pub_message(
                "pub-sub", object(), _PushStrategy.WAIT
            )
        with self.assertRaisesRegex(
            PyCGraphException,
            rf"^message connection \[{old_conn_id}\] not found$",
        ):
            self.manager.sub_message(old_conn_id, timeout_ms=0)

        new_conn_id = self.manager.bind_message_topic("new", capacity=1)
        self.assertGreater(new_conn_id, old_conn_id)

    def test_receive_timeout_includes_topic_and_duration(self):
        self.manager.create_message_topic("topic", capacity=1)

        with self.assertRaisesRegex(
            PyCGraphException,
            r"^receive message timeout, topic \[topic\], timeout \[0\] ms$",
        ):
            self.manager.recv_message("topic", timeout_ms=0)

    def test_subscribe_timeout_includes_connection_and_duration(self):
        conn_id = self.manager.bind_message_topic("topic", capacity=1)

        with self.assertRaisesRegex(
            PyCGraphException,
            rf"^subscribe message timeout, connection "
            rf"\[{conn_id}\], timeout \[0\] ms$",
        ):
            self.manager.sub_message(conn_id, timeout_ms=0)

    def test_missing_lifecycle_targets_report_not_found(self):
        with self.assertRaisesRegex(
            PyCGraphException, r"^message topic \[missing\] not found$"
        ):
            self.manager.remove_message_topic("missing")
        with self.assertRaisesRegex(
            PyCGraphException, r"^message topic \[missing\] not found$"
        ):
            self.manager.drop_message_topic("missing")
        with self.assertRaisesRegex(
            PyCGraphException, r"^message connection \[99\] not found$"
        ):
            self.manager.detach_message_subscription("missing", 99)


class GMessagePyFacadeTest(unittest.TestCase):
    def setUp(self):
        GMessagePy.clear_messages()

    def tearDown(self):
        GMessagePy.clear_messages()

    def test_public_types_have_expected_values_and_metadata(self):
        self.assertEqual(1, GMessagePy.PushStrategy.WAIT.value)
        self.assertEqual(2, GMessagePy.PushStrategy.REPLACE.value)
        self.assertEqual(3, GMessagePy.PushStrategy.DROP.value)
        self.assertTrue(issubclass(GMessagePy.Error, RuntimeError))
        self.assertIs(GMessagePy.Error, PyCGraphException)
        self.assertEqual("pycgraph", GMessagePy.__module__)
        self.assertEqual("pycgraph", GMessagePy.PushStrategy.__module__)
        self.assertEqual("pycgraph", GMessagePy.Error.__module__)
        self.assertEqual(
            "GMessagePy.PushStrategy",
            GMessagePy.PushStrategy.__qualname__,
        )
        self.assertEqual("GMessagePy.Error", GMessagePy.Error.__qualname__)

    def test_methods_are_static_and_default_to_wait(self):
        method_names = (
            "create_message_topic",
            "remove_message_topic",
            "send_message",
            "recv_message",
            "bind_message_topic",
            "pub_message",
            "sub_message",
            "detach_message_subscription",
            "drop_message_topic",
            "clear_messages",
        )
        for method_name in method_names:
            with self.subTest(method_name=method_name):
                self.assertIsInstance(
                    GMessagePy.__dict__[method_name], staticmethod
                )

        message = object()
        GMessagePy.create_message_topic("topic", capacity=1)
        self.assertEqual(1, GMessagePy.send_message("topic", message))
        self.assertIs(
            message,
            GMessagePy.recv_message("topic", timeout_ms=0),
        )

    def test_none_is_a_valid_payload_for_both_modes(self):
        GMessagePy.create_message_topic("send-recv", capacity=1)
        self.assertEqual(
            1, GMessagePy.send_message("send-recv", None)
        )
        self.assertIsNone(
            GMessagePy.recv_message("send-recv", timeout_ms=0)
        )

        conn_id = GMessagePy.bind_message_topic("pub-sub", capacity=1)
        self.assertEqual(1, GMessagePy.pub_message("pub-sub", None))
        self.assertIsNone(GMessagePy.sub_message(conn_id, timeout_ms=0))

    def test_empty_topic_is_valid_in_both_modes(self):
        message = object()
        GMessagePy.create_message_topic("", capacity=1)
        conn_id = GMessagePy.bind_message_topic("", capacity=1)

        self.assertEqual(1, GMessagePy.send_message("", message))
        self.assertIs(message, GMessagePy.recv_message("", timeout_ms=0))
        self.assertEqual(1, GMessagePy.pub_message("", message))
        self.assertIs(message, GMessagePy.sub_message(conn_id, timeout_ms=0))

    def test_topic_validation_is_shared_by_all_topic_methods(self):
        topic_calls = (
            lambda: GMessagePy.create_message_topic(1, 1),
            lambda: GMessagePy.remove_message_topic(1),
            lambda: GMessagePy.send_message(1, object()),
            lambda: GMessagePy.recv_message(1),
            lambda: GMessagePy.bind_message_topic(1, 1),
            lambda: GMessagePy.pub_message(1, object()),
            lambda: GMessagePy.detach_message_subscription(1, 1),
            lambda: GMessagePy.drop_message_topic(1),
        )
        for call in topic_calls:
            with self.subTest(call=call):
                with self.assertRaisesRegex(
                    GMessagePy.Error, r"^message topic must be str$"
                ):
                    call()

    def test_capacity_must_be_positive_non_boolean_int(self):
        for capacity in (0, -1, 1.0, True, None):
            for call in (
                lambda: GMessagePy.create_message_topic("send", capacity),
                lambda: GMessagePy.bind_message_topic("pub", capacity),
            ):
                with self.subTest(capacity=capacity, call=call):
                    with self.assertRaisesRegex(
                        GMessagePy.Error,
                        r"^message capacity must be an int "
                        r"greater than or equal to 1$",
                    ):
                        call()

    def test_timeout_must_be_none_or_non_negative_non_boolean_int(self):
        GMessagePy.create_message_topic("send", capacity=1)
        conn_id = GMessagePy.bind_message_topic("pub", capacity=1)

        for timeout_ms in (-1, 1.0, True, "1"):
            for call in (
                lambda: GMessagePy.recv_message("send", timeout_ms),
                lambda: GMessagePy.sub_message(conn_id, timeout_ms),
            ):
                with self.subTest(timeout_ms=timeout_ms, call=call):
                    with self.assertRaisesRegex(
                        GMessagePy.Error,
                        r"^message timeout must be None or an int "
                        r"greater than or equal to 0$",
                    ):
                        call()

    def test_connection_id_must_be_positive_non_boolean_int(self):
        for conn_id in (0, -1, 1.0, True, None):
            for call in (
                lambda: GMessagePy.sub_message(conn_id, timeout_ms=0),
                lambda: GMessagePy.detach_message_subscription(
                    "topic", conn_id
                ),
            ):
                with self.subTest(conn_id=conn_id, call=call):
                    with self.assertRaisesRegex(
                        GMessagePy.Error,
                        r"^message connection must be an int greater than 0$",
                    ):
                        call()

    def test_strategy_must_be_push_strategy_member(self):
        GMessagePy.create_message_topic("send", capacity=1)
        conn_id = GMessagePy.bind_message_topic("pub", capacity=1)

        for strategy in (1, None, "WAIT"):
            for call in (
                lambda: GMessagePy.send_message("send", object(), strategy),
                lambda: GMessagePy.pub_message("pub", object(), strategy),
            ):
                with self.subTest(strategy=strategy, call=call):
                    with self.assertRaisesRegex(
                        GMessagePy.Error,
                        r"^message push strategy must be "
                        r"GMessagePy\.PushStrategy$",
                    ):
                        call()

        GMessagePy.detach_message_subscription("pub", conn_id)

    def test_operational_failures_use_public_error_type(self):
        with self.assertRaises(GMessagePy.Error):
            GMessagePy.recv_message("missing", timeout_ms=0)


if __name__ == "__main__":
    unittest.main()
