"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyPubMessageNode
@Time: 2026/10/21 12:07
@Desc: 
"""

import time
from pycgraph import GNode, GMessage, GMessagePushStrategy, CStatus

from MyParams.MyMessageParam import MyMessageParam


class MyPubMessageNode(GNode):
    def __init__(self):
        super().__init__()
        self.val = 0

    def run(self):
        time.sleep(0.2)
        self.val += 1
        msg = MyMessageParam(self.val, "xxx" + str(self.val))
        return GMessage.pubMessage("pub-sub", msg, GMessagePushStrategy.WAIT)
