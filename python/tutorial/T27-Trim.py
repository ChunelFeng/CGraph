"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T27-Trim
@Time: 2025/5/1 20:32
@Desc: 
"""


from pycgraph import GPipeline

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2


def tutorial_trim():
    pipeline = GPipeline()
    a, b, c, d = MyNode1(), MyNode2(), MyNode1(), MyNode2()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {a, b, c}, "nodeD")

    trim_size = pipeline.trim()
    print('trim size is: {0}'.format(trim_size))

    skeleton = pipeline.dump()
    print(skeleton)


if __name__ == '__main__':
    tutorial_trim()
