"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T02-Cluster
@Time: 2025/2/22 11:40
@Desc: 
"""

from PyCGraph import GPipeline, GCluster, CStatus

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2


def tutorial_cluster():
    b1, b2, b3 = MyNode1("nodeB1"), MyNode1("nodeB2", 3), MyNode2("nodeB3")
    b_cluster = GCluster([b1, b2, b3])    # 入参为 list[] 表示写入 group(当前为 cluster) 中的内容，而非依赖内容

    pipeline = GPipeline()
    a, c, d = MyNode1(), MyNode2(), MyNode1()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b_cluster, {a}, "clusterB", 2)
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {b_cluster, c}, "nodeD", 2)

    pipeline.process()


if __name__ == '__main__':
    tutorial_cluster()
