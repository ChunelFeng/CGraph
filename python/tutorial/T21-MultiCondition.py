"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T21-MultiCondition
@Time: 2025/3/7 23:52
@Desc: 
"""

from pycgraph import GPipeline, GSerialMultiCondition, GParallelMultiCondition

from MyGNode.MyMatchNode import MyMatchNode
from MyGNode.MyNode1 import MyNode1
from MyGNode.MyWriteParamNode import MyWriteParamNode


def tutorial_multi_condition():
    a, c = MyWriteParamNode(), MyWriteParamNode()
    b_multi_condition = GSerialMultiCondition([MyNode1('nodeB1'), MyMatchNode('nodeB2')])
    d_multi_condition = GParallelMultiCondition([MyMatchNode('nodeD1'), MyMatchNode('nodeD2')])

    pipeline = GPipeline()
    pipeline.registerGElement(a, set(), 'nodeA')
    pipeline.registerGElement(b_multi_condition, {a}, 'multiConditionB')
    pipeline.registerGElement(c, {b_multi_condition}, 'nodeC')
    pipeline.registerGElement(d_multi_condition, {c}, 'multiConditionD')

    pipeline.process()


if __name__ == '__main__':
    tutorial_multi_condition()
