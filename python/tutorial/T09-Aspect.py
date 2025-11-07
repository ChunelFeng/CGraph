"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T09-Aspect
@Time: 2025/3/5 23:31
@Desc: 
"""

from pycgraph import GPipeline, GCluster

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2
from MyGAspect.MyTimerAspect import MyTimerAspect


def tutorial_aspect():
    a, b1, b2, c = MyNode1(), MyNode2('nodeB1'), MyNode1('nodeB2'), MyNode2()
    b_cluster = GCluster([b1, b2])

    a.addGAspect(MyTimerAspect())
    b_cluster.addGAspect(MyTimerAspect())

    pipeline = GPipeline()
    pipeline.registerGElement(a, set(), 'nodeA')
    pipeline.registerGElement(b_cluster, {a}, 'regionB')
    pipeline.registerGElement(c, {b_cluster}, 'nodeC')
    pipeline.process()


if __name__ == '__main__':
    tutorial_aspect()
