"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyParam
@Time: 2025/2/27 23:37
@Desc: 
"""

from pycgraph import GParam, CStatus

class MyParam(GParam):
    value = 0
    count = 0

    def reset(self, curStatus: CStatus):
        self.value = 0
        return
