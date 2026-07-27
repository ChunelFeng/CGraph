"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T16-MessageSendRecv
@Time: 2026/7/26 15:15
@Desc: 在两个 pipeline 之间通过 send/recv 传递消息

阻塞的 message 操作会占用当前 CGraph worker thread。在线程池过小，
或 DAG 依赖要求对端必须等当前节点完成后才能运行时，
WAIT 可能造成业务死锁。
"""

import threading

from pycgraph import GMessage, GPipeline

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2
from MyGNode.MyRecvMessageNode import MyRecvMessageNode
from MyGNode.MySendMessageNode import MySendMessageNode


def send_message():
    pipeline = GPipeline()
    node1 = MyNode1()
    send_node = MySendMessageNode()

    pipeline.registerGElement(node1, set(), "myNode1")
    pipeline.registerGElement(send_node, {node1}, "MySendMessageNode")
    return pipeline.process(3)


def recv_message():
    pipeline = GPipeline()
    node2 = MyNode2()
    recv_node = MyRecvMessageNode()

    pipeline.registerGElement(node2, set(), "myNode2")
    pipeline.registerGElement(recv_node, {node2}, "MyRecvMessageNode")
    return pipeline.process(3)


def _run_concurrently(functions):
    statuses = [None] * len(functions)
    errors = []

    def execute(index, function):
        try:
            statuses[index] = function()
        except BaseException as error:
            errors.append(error)

    threads = [
        threading.Thread(target=execute, args=(index, function))
        for index, function in enumerate(functions)
    ]
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join()

    if errors:
        raise errors[0]
    for status in statuses:
        if not status.isOK():
            raise RuntimeError(status.getInfo())


def tutorial_message_send_recv():
    topic = "send-recv"
    GMessage.create_message_topic(topic, capacity=48)

    try:
        _run_concurrently((send_message, recv_message))

        try:
            GMessage.recv_message(topic, timeout_ms=10)
        except GMessage.Error as error:
            print("handled timeout:", error)
    finally:
        GMessage.clear_messages()


if __name__ == "__main__":
    tutorial_message_send_recv()
