"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T03-Region
@Time: 2025/2/24 22:52
@Desc: 
"""

from PyCGraph import GNode, GPipeline, GRegion, CStatus

from MyPyGNode.MyPyNode1 import MyPyNode1
from MyPyGNode.MyPyNode2 import MyPyNode2

def tutorial_region():
    b1 = MyPyNode1("nodeB1")
    b2 = MyPyNode2({b1}, "nodeB2", 2)
    b3 = MyPyNode1({b1}, "nodeB3", 1)
    b4 = MyPyNode1({b2, b3}, "nodeB4", 1)
    b_region = GRegion()
    b_region.addGElements([b1, b2, b3, b4])

    pipeline = GPipeline()
    a, c, d = MyPyNode1(), MyPyNode2(), MyPyNode1()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b_region, {a}, "regionB", 2)
    pipeline.registerGElement(c, {b_region}, "nodeC")

    pipeline.process()


if __name__ == '__main__':
    tutorial_region()
