"""
Topic registries and lifecycle management for the pycgraph message runtime.
"""

import threading

from _pycgraph_message_primitives import (
    _LocalMessageQueue,
    PyCGraphException,
)


class _SendRecvTopic:
    def __init__(self, capacity):
        self.queue = _LocalMessageQueue(capacity)


class _Subscription:
    __slots__ = ("topic", "queue")

    def __init__(self, topic, queue):
        self.topic = topic
        self.queue = queue


class _PubSubTopic:
    def __init__(self):
        self.subscribers = {}
        self.subscriber_queues = ()

    def add(self, conn_id, queue):
        self.subscribers[conn_id] = queue
        self.subscriber_queues = tuple(self.subscribers.values())

    def remove(self, conn_id):
        del self.subscribers[conn_id]
        self.subscriber_queues = tuple(self.subscribers.values())


class _LocalMessageManager:
    """Owns all message topics in the current Python interpreter."""

    def __init__(self):
        self._send_recv_topics = {}
        self._pub_sub_topics = {}
        self._subscriptions = {}
        self._next_conn_id = 1
        self._registry_lock = threading.Lock()

    @staticmethod
    def _topic_not_found(topic):
        return PyCGraphException("message topic [{}] not found".format(topic))

    @staticmethod
    def _connection_not_found(conn_id):
        return PyCGraphException(
            "message connection [{}] not found".format(conn_id)
        )

    def create_message_topic(self, topic, capacity):
        with self._registry_lock:
            existing = self._send_recv_topics.get(topic)
            if existing is None:
                self._send_recv_topics[topic] = _SendRecvTopic(capacity)
                return
            if existing.queue.capacity != capacity:
                raise PyCGraphException(
                    "message topic [{}] capacity mismatch, "
                    "existing [{}], requested [{}]".format(
                        topic, existing.queue.capacity, capacity
                    )
                )

    def remove_message_topic(self, topic):
        with self._registry_lock:
            if topic not in self._send_recv_topics:
                raise self._topic_not_found(topic)
            del self._send_recv_topics[topic]

    def send_message(self, topic, message, strategy):
        send_recv_topic = self._send_recv_topics.get(topic)
        if send_recv_topic is None:
            raise self._topic_not_found(topic)
        return send_recv_topic.queue.push(message, strategy)

    def recv_message(self, topic, timeout_ms):
        send_recv_topic = self._send_recv_topics.get(topic)
        if send_recv_topic is None:
            raise self._topic_not_found(topic)
        timeout_error = (
            "receive message timeout, topic [{}], timeout [{}] ms".format(
                topic, timeout_ms
            )
        )
        return send_recv_topic.queue.pop(timeout_ms, timeout_error)

    def bind_message_topic(self, topic, capacity):
        with self._registry_lock:
            pub_sub_topic = self._pub_sub_topics.get(topic)
            if pub_sub_topic is None:
                pub_sub_topic = _PubSubTopic()
                self._pub_sub_topics[topic] = pub_sub_topic

            conn_id = self._next_conn_id
            self._next_conn_id += 1
            queue = _LocalMessageQueue(capacity)
            pub_sub_topic.add(conn_id, queue)
            self._subscriptions[conn_id] = _Subscription(topic, queue)
            return conn_id

    def pub_message(self, topic, message, strategy):
        pub_sub_topic = self._pub_sub_topics.get(topic)
        if pub_sub_topic is None:
            raise self._topic_not_found(topic)

        pushed = 0
        for queue in pub_sub_topic.subscriber_queues:
            pushed += queue.push(message, strategy)
        return pushed

    def sub_message(self, conn_id, timeout_ms):
        suber = self._subscriptions.get(conn_id)
        if suber is None:
            raise self._connection_not_found(conn_id)
        timeout_error = (
            "subscribe message timeout, connection [{}], "
            "timeout [{}] ms".format(conn_id, timeout_ms)
        )
        return suber.queue.pop(timeout_ms, timeout_error)

    def detach_message_subscription(self, topic, conn_id):
        with self._registry_lock:
            suber = self._subscriptions.get(conn_id)
            if suber is None:
                raise self._connection_not_found(conn_id)
            if suber.topic != topic:
                raise PyCGraphException(
                    "message connection [{}] does not belong to "
                    "topic [{}]".format(conn_id, topic)
                )

            self._pub_sub_topics[topic].remove(conn_id)
            del self._subscriptions[conn_id]

    def drop_message_topic(self, topic):
        with self._registry_lock:
            pub_sub_topic = self._pub_sub_topics.get(topic)
            if pub_sub_topic is None:
                raise self._topic_not_found(topic)
            for conn_id in pub_sub_topic.subscribers:
                del self._subscriptions[conn_id]
            del self._pub_sub_topics[topic]

    def clear_messages(self):
        with self._registry_lock:
            self._send_recv_topics.clear()
            self._pub_sub_topics.clear()
            self._subscriptions.clear()
