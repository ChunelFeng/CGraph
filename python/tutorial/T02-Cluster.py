"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T02-Cluster
@Time: 2025/2/22 11:40
@Desc: 
"""

from PyCGraph import GNode, GPipeline, GCluster, CStatus

from MyPyGNode.MyPyNode1 import MyPyNode1
from MyPyGNode.MyPyNode2 import MyPyNode2

def tutorial_cluster():
    b1, b2, b3 = MyPyNode1("nodeB1"), MyPyNode1("nodeB2", 3), MyPyNode2("nodeB3")
    b_cluster = GCluster()
    b_cluster.addGElements([b1, b2, b3])

    pipeline = GPipeline()
    a, c, d = MyPyNode1(), MyPyNode2(), MyPyNode1()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b_cluster, {a}, "clusterB", 2)
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {b_cluster, c}, "nodeD", 2)

    pipeline.process()


if __name__ == '__main__':
    tutorial_cluster()
