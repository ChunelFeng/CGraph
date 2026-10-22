"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyParam
@Time: 2025/2/27 23:37
@Desc: 
"""

from pycgraph import param, CStatus

@param
class MyParam:
    value = 0
    count = 0

    def reset(self, curStatus: CStatus):
        self.value = 0
        return
