"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MySendMessageNode
@Time: 2026/7/26 15:10
@Desc: Send a message from a CGraph node
"""

from pycgraph import CStatus, GMessage, GNode

from MyParams.MyMessageParam import MyMessageParam


class MySendMessageNode(GNode):
    def __init__(self):
        super().__init__()
        self._num = 0

    def run(self):
        message = MyMessageParam()
        message.num = self._num * 10
        message.info = "this is a test send info, num = {}".format(
            message.num
        )
        self._num += 1

        try:
            GMessage.send_message(
                "send-recv",
                message,
                GMessage.PushStrategy.WAIT,
            )
        except GMessage.Error as error:
            return CStatus(-1, str(error))
        return CStatus()
