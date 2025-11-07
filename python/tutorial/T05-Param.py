"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T05-Param
@Time: 2025/2/27 23:41
@Desc: 
"""

from pycgraph import GNode, GPipeline

from MyGNode.MyWriteParamNode import MyWriteParamNode
from MyGNode.MyReadParamNode import MyReadParamNode


def tutorial_param():
    pipeline = GPipeline()
    a, b, e = MyReadParamNode(), MyReadParamNode(), MyReadParamNode()
    c, d, f = MyWriteParamNode(), MyWriteParamNode(), MyWriteParamNode()

    pipeline.registerGElement(a, set(), "readNodeA")
    pipeline.registerGElement(b, {a}, "readNodeB")
    pipeline.registerGElement(c, {a}, "writeNodeC")
    pipeline.registerGElement(d, {a}, "writeNodeD", 2)
    pipeline.registerGElement(e, {a}, "readNodeE")
    pipeline.registerGElement(f, {b, c, d, e}, "writeNodeF")

    pipeline.init()

    for i in range(0, 3):
        status = pipeline.run()
        print('---- tutorial_param, loop : {0}, and run status = {1}'.format(i + 1, status.getCode()))

    pipeline.destroy()


if __name__ == '__main__':
    tutorial_param()
