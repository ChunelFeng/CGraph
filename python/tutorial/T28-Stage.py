"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T28-Stage
@Time: 2025/4/4 01:08
@Desc: 
"""

from pycgraph import GPipeline, GStage

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyStageNode import MyStageNode


def tutorial_stage():
    pipeline = GPipeline()
    a, b, c, d, e = MyNode1(), MyStageNode(1, 2), MyStageNode(3, 1), MyStageNode(3, 1), MyNode1()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {a}, "nodeD")
    pipeline.registerGElement(e, {b, c, d}, "nodeE")

    stage = GStage()
    pipeline.addGStage(stage, 'stage1', 3)

    pipeline.process()


if __name__ == '__main__':
    tutorial_stage()
