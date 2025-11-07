"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyParamCondition
@Time: 2025/3/2 13:49
@Desc: 
"""


from pycgraph import GCondition, GElementRelation

from MyParams.MyParam import MyParam

class MyParamCondition(GCondition):
    def choose(self):
        param: MyParam = self.getGParamWithNoEmpty("param1")
        param.lock()
        cnt = param.count
        param.unlock()

        relation: GElementRelation = self.getRelation()
        return cnt % len(relation.children)
