"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyStageNode
@Time: 2025/4/4 01:16
@Desc: 
"""

import time
from datetime import datetime

from pycgraph import GNode, CStatus


class MyStageNode(GNode):
    def __init__(self, before: int, after: int):
        super().__init__()
        self._before = before
        self._after = after

    def run(self):
        time.sleep(self._before)
        print('[{0}] ----> {1} wait {2}s before enter stage'.format(datetime.now(), self.getName(), self._before))

        status: CStatus = self.enterStage('stage1')
        print('[{0}] <---- {1} wait {2}s after enter stage'.format(datetime.now(), self.getName(), self._after))
        time.sleep(self._after)
        return status
