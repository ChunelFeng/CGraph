"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T10-AspectParam
@Time: 2025/3/30 21:11
@Desc: 
"""

from pycgraph import GPipeline

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2
from MyGAspect.MyConnApsect import MyConnAspect


def tutorial_aspect_param():
    a, b, c, d = MyNode1(), MyNode2(), MyNode1(), MyNode2()

    a.addGAspect(MyConnAspect('127.0.0.1', 6666))
    d.addGAspect(MyConnAspect('255.255.255.255', 9999))

    pipeline = GPipeline()
    pipeline.registerGElement(a, set(), 'nodeA')
    pipeline.registerGElement(b, {a}, 'nodeB')
    pipeline.registerGElement(c, {b}, 'nodeC')
    pipeline.registerGElement(d, {c}, 'nodeD')
    pipeline.process()


if __name__ == '__main__':
    tutorial_aspect_param()
