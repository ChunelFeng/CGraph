"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyMessageParam
@Time: 2026/10/21 11:23
@Desc:
"""

from pycgraph import GMessageParam


class MyMessageParam(GMessageParam):
    def __init__(self, num, info):
        super().__init__()
        self.num = num
        self.info = info
