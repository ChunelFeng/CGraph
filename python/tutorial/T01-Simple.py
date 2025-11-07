"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T01-Simple
@Time: 2025/2/21 23:57
@Desc: 
"""

from pycgraph import GPipeline, CStatus

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2


def tutorial_simple():
    pipeline = GPipeline()
    a, b, c, d = MyNode1(), MyNode2(), MyNode1(), MyNode2()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {b, c}, "nodeD")

    status: CStatus = pipeline.init()
    if status.isErr():
        # please check api return status
        # ret_code == 0 is ok, default
        # ret_code < 0 means error, ex: return CStatus(-1, "my error info")
        print('pipeline init failed, error code is {0}, error info is {1}'.format(
            status.getCode(), status.getInfo()))
        return

    for i in range(0, 3):
        status = pipeline.run()
        print("==== tutorial_simple, loop: {0}, and run status = {1}".format(i + 1, status.getCode()))

    pipeline.destroy()


if __name__ == '__main__':
    tutorial_simple()
