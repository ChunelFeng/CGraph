"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MySubMessageNode
@Time: 2026/7/26 15:10
@Desc: Subscribe to messages from a CGraph node
"""

from pycgraph import CStatus, GMessagePy, GNode


class MySubMessageNode(GNode):
    def __init__(self, conn_id):
        super().__init__()
        self._conn_id = conn_id

    def init(self):
        print(
            "==> [{}] [init] get conn id = [{}]".format(
                self.getName(),
                self._conn_id,
            )
        )
        return CStatus()

    def run(self):
        try:
            message = GMessagePy.sub_message(self._conn_id)
        except GMessagePy.Error as error:
            print("[{}] recv message error: {}".format(self.getName(), error))
            return CStatus(-1, str(error))

        print(
            "[{}] conn id = [{}], num = [{}], info = [{}]".format(
                self.getName(),
                self._conn_id,
                message.num,
                message.info,
            )
        )
        return CStatus()
