"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyRecvMessageNode
@Time: 2026/7/26 15:10
@Desc: Receive a message from a CGraph node
"""

from pycgraph import CStatus, GMessagePy, GNode


class MyRecvMessageNode(GNode):
    def run(self):
        try:
            message = GMessagePy.recv_message("send-recv")
        except GMessagePy.Error as error:
            print("MyRecvMessageNode recv message error:", error)
            return CStatus(-1, str(error))

        print(
            "num = [{}], info = [{}]".format(
                message.num,
                message.info,
            )
        )
        return CStatus()
