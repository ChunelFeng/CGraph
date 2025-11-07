"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T13-Daemon
@Time: 2025/3/30 00:46
@Desc: 
"""

from pycgraph import GPipeline

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyWriteParamNode import MyWriteParamNode
from MyGDaemon.MyMonitorDaemon import MyMonitorGDaemon


def tutorial_daemon():
    pipeline = GPipeline()
    a, b = MyNode1(), MyWriteParamNode()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "writeParamNodeB")

    pipeline.addGDaemon(MyMonitorGDaemon(), ms=3333)

    pipeline.process(20)


if __name__ == '__main__':
    tutorial_daemon()

