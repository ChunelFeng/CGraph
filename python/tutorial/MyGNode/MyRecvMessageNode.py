"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyRecvMessageNode
@Time: 2026/10/21 11:23
@Desc: 
"""

from pycgraph import GNode, GMessage

class MyRecvMessageNode(GNode):
    def run(self):
        status, msg = GMessage.recvMessage("send-recv", timeout_ms=1000)

        if status:
            print("received message: num = {}, info = {}".format(msg.num, msg.info))
        return status
