"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T22-Timeout
@Time: 2025/3/30 13:53
@Desc: 
"""

from pycgraph import GPipeline, CStatus

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2


def tutorial_timeout():
    pipeline = GPipeline()
    a, b, c, d = MyNode1(), MyNode2(), MyNode1(), MyNode1()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {b, c}, "nodeD")

    status: CStatus = pipeline.process()
    if status.isOK():
        print("---- T22-timeout.py pipeline run finish \n")

    c.setTimeout(300)
    print("---- set [{0}] timeout value to 300".format(c.getName()))
    status = pipeline.process()
    if not status.isOK():
        print("---- T22-timeout pipeline run error info : {0} \n".format(status.getInfo()))

    c.setTimeout(0)
    print("---- set [{0}] no timeout, rerun pipeline again".format(c.getName()))
    status = pipeline.process()
    if status.isOK():
        print("---- T22-timeout.py pipeline run finish again")


if __name__ == '__main__':
    tutorial_timeout()
