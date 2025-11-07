"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyTemplateNode
@Time: 2025/4/2 00:02
@Desc: 
"""

from pycgraph import GNode, CStatus


class MyTemplateNode(GNode):
    def __init__(self, num: int, score: float):
        super().__init__()    # must call super init function
        self._num = num
        self._score = score

    def run(self):
        print('[{0}] num = [{1}], score = [{2}]'.format(self.getName(), self._num, self._score))
        return CStatus()
