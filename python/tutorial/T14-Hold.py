"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T14-Hold
@Time: 2025/3/4 00:15
@Desc: 
"""

from pycgraph import GPipeline

from MyGNode.MyHoldNode import MyHoldNode
from MyGNode.MyNode1 import MyNode1


def tutorial_hold():
    pipeline = GPipeline()
    a, b = MyHoldNode(), MyNode1()

    pipeline.registerGElement(a, set(), 'myHold')
    pipeline.registerGElement(b, {a}, 'nodeA')
    pipeline.process(3)


if __name__ == '__main__':
    tutorial_hold()

