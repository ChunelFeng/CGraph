"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T17-MessagePubSub
@Time: 2026/10/21 13:15
@Desc:
"""

from pycgraph import GMessage, GPipeline

from MyGNode.MySubMessageNode import MySubMessageNode
from MyGNode.MyPubMessageNode import MyPubMessageNode
from MyGNode.MyNode1 import MyNode1
from MyParams.MyMessageParam import MyMessageParam


def build_pub_pipeline():
    p = GPipeline()
    a, pn = MyNode1(), MyPubMessageNode()
    p.registerGElement(a, set(), "nodeA")
    p.registerGElement(pn, {a}, "MyPubMessageNode")
    return p


def build_sub_pipeline():
    p = GPipeline()
    conn_id = GMessage.bindTopic("pub-sub", MyMessageParam, capacity=8)
    a, sn = MyNode1(), MySubMessageNode(conn_id)
    p.registerGElement(a, set(), "node" + str(conn_id))
    p.registerGElement(sn, {a}, "MySubMessageNode")
    return p


def tutorial_message_pub_sub():
    pub_pipeline = build_pub_pipeline()
    sub_pipelines = [build_sub_pipeline() for _ in range(3)]

    times = 5
    futures = [sp.asyncProcess(times) for sp in sub_pipelines]
    futures.append(pub_pipeline.asyncProcess(times))

    for fut in futures:
        status = fut.get()
        if not status:
            print("tutorial_message_pub_sub run failed, err info is: {}".format(status.getInfo()))

    GMessage.clear()


if __name__ == "__main__":
    tutorial_message_pub_sub()
