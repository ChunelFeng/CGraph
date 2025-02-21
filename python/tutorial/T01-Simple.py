"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T01-Simple
@Time: 2025/2/21 23:57
@Desc: 
"""

from PyCGraph import GNode, GPipeline, CStatus

from MyPyGNode.MyPyNode1 import MyPyNode1
from MyPyGNode.MyPyNode2 import MyPyNode2

def tutorial_simple():
    pipeline = GPipeline()
    a, b, c, d = MyPyNode1(), MyPyNode2(), MyPyNode1(), MyPyNode2()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {b, c}, "nodeD")
    pipeline.init()

    for i in range(0, 3):
        status = pipeline.run()
        print("==== tutorial_simple, loop: {0}, and run status = {1}".format(i + 1, status.getCode()))

    pipeline.destroy()


if __name__ == '__main__':
    tutorial_simple()
