"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MySendMessageNode
@Time: 2026/10/21 11:29
@Desc: 
"""
import time
from pycgraph import GNode, GMessage, GMessagePushStrategy

from MyParams.MyMessageParam import MyMessageParam

class MySendMessageNode(GNode):
    def __init__(self):
        super().__init__()
        self.val = 0

    def run(self):
        time.sleep(0.5)
        message = MyMessageParam(self.val, "xxx" + str(self.val))
        self.val += 1
        return GMessage.sendMessage("send-recv", message, GMessagePushStrategy.WAIT)
