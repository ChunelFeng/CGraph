"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T08-Template
@Time: 2025/4/2 00:01
@Desc:
"""

from pycgraph import GPipeline

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyTemplateNode import MyTemplateNode


def tutorial_template():
    pipeline = GPipeline()
    a, b, c = MyNode1(), MyTemplateNode(1, 3.25), MyTemplateNode(2, 9.28)

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "templateNodeB")
    pipeline.registerGElement(c, {b}, "templateNodeC")

    pipeline.process()


if __name__ == '__main__':
    tutorial_template()
