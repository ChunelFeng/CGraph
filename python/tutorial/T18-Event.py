"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T18-Event
@Time: 2025/3/22 00:21
@Desc: 
"""

from pycgraph import GPipeline

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyEventNode import MyEventNode
from MyGNode.MyWriteParamNode import MyWriteParamNode
from MyGEvent.MyPrintEvent import MyPrintEvent


def tutorial_event():
    pipeline = GPipeline()
    a, b, c, d = MyWriteParamNode(), MyEventNode(), MyNode1(), MyEventNode()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "eventNodeB")
    pipeline.registerGElement(c, {b}, "nodeC")
    pipeline.registerGElement(d, {c}, "eventNodeD")

    pipeline.addGEvent(MyPrintEvent(), "event1")

    pipeline.process()


if __name__ == '__main__':
    tutorial_event()
