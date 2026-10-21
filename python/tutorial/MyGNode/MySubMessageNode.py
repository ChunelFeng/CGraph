"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MySubMessageNode
@Time: 2026/10/21 12:07
@Desc: 
"""

from pycgraph import GNode, GMessage, CStatus


class MySubMessageNode(GNode):
    def __init__(self, conn_id):
        super().__init__()
        self.conn_id = conn_id

    def init(self):
        print("MySubMessageNode conn_id is [{0}]".format(self.conn_id))
        return CStatus()

    def run(self):
        status, msg = GMessage.subMessage(self.conn_id, timeout_ms=1000)
        if status:
            print("subscriber {} received: num {}, info {}".format(self.conn_id, msg.num, msg.info))
        return status
