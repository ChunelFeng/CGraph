"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T24-Fence
@Time: 2025/4/10 00:17
@Desc: 
"""

from pycgraph import GPipeline, GFence, GElementTimeoutStrategy

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2


def tutorial_fence():
    pipeline = GPipeline()
    a, b, c, e = MyNode2(), MyNode1(), MyNode1(), MyNode1()
    d_fence = GFence()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d_fence, {b, c}, "fenceD")
    pipeline.registerGElement(e, {d_fence}, "nodeE")

    a.setTimeout(200, GElementTimeoutStrategy.HOLD_BY_PIPELINE)
    c.setTimeout(300, GElementTimeoutStrategy.HOLD_BY_PIPELINE)
    d_fence.waitGElements({a, c})    # wait until a & c run finished

    pipeline.process()


if __name__ == '__main__':
    tutorial_fence()
