"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyHoldNode
@Time: 2025/3/4 00:08
@Desc: 
"""

from PyCGraph import GNode, CStatus

from MyParams.MyParam import MyParam

class MyHoldNode(GNode):
    param_key = 'hold-param'
    def init(self):
        return self.createGParam(MyParam(), self.param_key)

    def run(self):
        param: MyParam = self.getGParam(self.param_key)
        param.value += 1
        print('current value is {0}'.format(param.value))
        return CStatus()

    def isHold(self):
        param: MyParam = self.getGParam(self.param_key)
        print('enter hold path, value is {0}'.format(param.value))
        return param.value < 5
