"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T04-Complex
@Time: 2025/3/2 23:43
@Desc: 
"""

from pycgraph import GPipeline, GCluster, GRegion

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2


def tutorial_complex():
    pipeline = GPipeline()
    b1, b2, b3 = MyNode1('nodeB1'), MyNode1('nodeB2'), MyNode2('nodeB3')
    b_cluster = GCluster([b1, b2, b3])

    d1, d2, d3 = MyNode1('nodeD1'), MyNode1('nodeD2'), MyNode1('nodeD3')
    d4 = MyNode2({d1}, 'nodeD4', 1)
    d23_cluster = GCluster([d2, d3])    # 表示 d23_cluster 中包含 [d2,d3] 两个element
    d23_cluster.addDependGElements({d1})    # 表示 d23_cluster 依赖 {d1} 一个element，注意 [] 和 {} 的不用用法
    d_region = GRegion([d1, d23_cluster, d4])

    a, c, e = MyNode1(), MyNode1(), MyNode1()
    pipeline.registerGElement(a, set(), 'nodeA')
    pipeline.registerGElement(b_cluster, set(), 'clusterB')
    pipeline.registerGElement(c, {a, b_cluster}, 'nodeC')
    pipeline.registerGElement(d_region, {a, b_cluster}, 'regionD', 2)
    pipeline.registerGElement(e, {c, d_region}, 'nodeE')

    pipeline.process()


if __name__ == '__main__':
    tutorial_complex()
