"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T16-MessageSendRecv
@Time: 2026/10/21 13:15
@Desc:
"""

from pycgraph import GMessage, GPipeline

from MyParams.MyMessageParam import MyMessageParam
from MyGNode.MyRecvMessageNode import MyRecvMessageNode
from MyGNode.MySendMessageNode import MySendMessageNode


def tutorial_message_send_recv():
    status = GMessage.createTopic("send-recv", MyMessageParam, capacity=8)
    if not status:
        raise RuntimeError(status.getInfo())

    p1, p2 = GPipeline(), GPipeline()
    sn, rn = MySendMessageNode(), MyRecvMessageNode()

    p1.registerGElement(sn)
    p2.registerGElement(rn)

    send_fut = p1.asyncProcess(5)
    recv_fut = p2.asyncProcess(5)

    status += send_fut.get()
    status += recv_fut.get()

    GMessage.clear()


if __name__ == "__main__":
    tutorial_message_send_recv()
