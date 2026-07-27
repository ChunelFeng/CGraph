"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyPubMessageNode
@Time: 2026/7/26 15:10
@Desc: Publish a message from a CGraph node
"""

import time

from pycgraph import CStatus, GMessage, GNode

from MyParams.MyMessageParam import MyMessageParam


class MyPubMessageNode(GNode):
    def __init__(self):
        super().__init__()
        self._num = 0

    def init(self):
        sleep_ms = 300
        time.sleep(sleep_ms / 1000.0)
        print(
            "==> [MyPubMessageNode] [init], sleep [{}] ms, "
            "to make sure sub node prepare ok.".format(sleep_ms)
        )
        return CStatus()

    def run(self):
        message = MyMessageParam()
        message.num = self._num * 100
        message.info = "this is a test info, num = {}".format(message.num)
        self._num += 1

        try:
            GMessage.pub_message(
                "pub-sub",
                message,
                GMessage.PushStrategy.WAIT,
            )
        except GMessage.Error as error:
            return CStatus(-1, str(error))
        return CStatus()
