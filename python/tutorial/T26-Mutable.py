"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T26-GMutable
@Time: 2025/4/9 00:04
@Desc: 
"""

from pycgraph import GPipeline

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyWriteParamNode import MyWriteParamNode
from MyGMutable.MyMutable import MyMutable


def tutorial_mutable():
    pipeline = GPipeline()
    a, c, d = MyNode1(), MyNode1(), MyWriteParamNode()
    b_mutable = MyMutable([MyNode1('nodeB1'), MyNode1('nodeB2'), MyNode1('nodeB3')])

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b_mutable, {a}, "nodeB")
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {b_mutable, c}, "nodeD")

    pipeline.process(6)


if __name__ == '__main__':
    tutorial_mutable()
