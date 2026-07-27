"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T17-MessagePubSub
@Time: 2026/7/26 15:15
@Desc: 一个 publisher pipeline 向三个 subscriber pipeline 发布消息

publish 只向每个 subscriber queue 复制同一个 Python 对象引用。发布后，
publisher 和 subscriber 都应把 payload 当作只读对象。

阻塞的 message 操作会占用当前 CGraph worker thread。在线程池过小，
或 DAG 依赖要求对端必须等当前节点完成后才能运行时，
WAIT 可能造成业务死锁。
"""

import threading

from pycgraph import GMessage, GPipeline

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2
from MyGNode.MyPubMessageNode import MyPubMessageNode
from MyGNode.MySubMessageNode import MySubMessageNode


def pub_message():
    pipeline = GPipeline()
    node1 = MyNode1()
    pub_node = MyPubMessageNode()

    pipeline.registerGElement(node1, set(), "myNode1")
    pipeline.registerGElement(pub_node, set(), "MyPubMessageNode")
    return pipeline.process(5)


def sub_message(conn_ids):
    conn_id = GMessage.bind_message_topic("pub-sub", capacity=1024)
    conn_ids.append(conn_id)

    pipeline = GPipeline()
    node2 = MyNode2()
    sub_node = MySubMessageNode(conn_id)

    pipeline.registerGElement(node2, set(), "myNode2")
    pipeline.registerGElement(sub_node, {node2}, "MySubMessageNode")
    return pipeline.process(5)


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


def tutorial_message_pub_sub():
    conn_ids = []

    def subscriber():
        return sub_message(conn_ids)

    try:
        _run_concurrently(
            (pub_message, subscriber, subscriber, subscriber)
        )

        try:
            GMessage.sub_message(conn_ids[0], timeout_ms=10)
        except GMessage.Error as error:
            print("handled timeout:", error)
    finally:
        GMessage.clear_messages()


if __name__ == "__main__":
    tutorial_message_pub_sub()
