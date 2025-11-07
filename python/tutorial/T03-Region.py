"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T03-Region
@Time: 2025/2/24 22:52
@Desc: 
"""

from pycgraph import GPipeline, GRegion

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2


def tutorial_region():
    b1 = MyNode1("nodeB1")
    b2 = MyNode2({b1}, "nodeB2", 2)
    b3 = MyNode1({b1}, "nodeB3", 1)
    b4 = MyNode1({b2, b3}, "nodeB4", 1)
    b_region = GRegion([b1, b2, b3, b4])

    pipeline = GPipeline()
    a, c = MyNode1(), MyNode2()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b_region, {a}, "regionB", 2)
    pipeline.registerGElement(c, {b_region}, "nodeC")

    pipeline.process()


if __name__ == '__main__':
    tutorial_region()
