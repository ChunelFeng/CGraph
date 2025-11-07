"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T19-Cancel
@Time: 2025/3/28 22:57
@Desc: 
"""

import time

from pycgraph import GPipeline, StdFutureCStatus

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2


def tutorial_cancel():
    pipeline = GPipeline()
    a, b, c, d = MyNode1(), MyNode2(), MyNode1(), MyNode2()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {b, c}, "nodeD")

    pipeline.init()

    result: StdFutureCStatus = pipeline.asyncRun()
    print("pipeline async run first time, BEGIN.")
    result.wait()
    print("pipeline async run first time, FINISH.")
    print("======================")

    result = pipeline.asyncRun()
    time.sleep(1.5)
    pipeline.cancel()
    print("pipeline async run second time, CANCEL.")
    print("======================")
    result.wait()

    pipeline.destroy()


if __name__ == '__main__':
    tutorial_cancel()