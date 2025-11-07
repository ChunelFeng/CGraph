"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyReadParamNode
@Time: 2025/2/27 23:42
@Desc: 
"""

from pycgraph import GNode, CStatus

from MyParams.MyParam import MyParam


class MyReadParamNode(GNode):
    def run(self):
        param = self.getGParamWithNoEmpty("param1")
        param.lock()
        print('[{0}] value is {1}, count is {2}'.format(self.getName(), param.value, param.count))
        param.unlock()
        return CStatus()
