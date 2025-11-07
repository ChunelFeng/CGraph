"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T20-Suspend
@Time: 2025/3/29 00:23
@Desc: 
"""

import time

from pycgraph import GPipeline, StdFutureCStatus

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2


def tutorial_suspend():
    pipeline = GPipeline()
    a, b, c, d = MyNode1(), MyNode2(), MyNode1(), MyNode2()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {b, c}, "nodeD")

    pipeline.init()

    result: StdFutureCStatus = pipeline.asyncRun()
    print("pipeline async run first time, BEGIN.")
    time.sleep(2.6)
    print("pipeline async run first time, SUSPEND.")
    pipeline.suspend()

    time.sleep(7.2)
    print("pipeline async run, RESUME after 7200ms.")
    pipeline.resume()
    result.wait()

    pipeline.destroy()


if __name__ == '__main__':
    tutorial_suspend()