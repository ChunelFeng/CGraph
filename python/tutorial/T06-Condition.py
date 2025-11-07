"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T06-Condition
@Time: 2025/2/25 23:34
@Desc: 
"""

from pycgraph import GPipeline

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2
from MyGCondition.MyCondition import MyCondition
from MyGCondition.MyParamCondition import MyParamCondition
from MyGNode.MyWriteParamNode import MyWriteParamNode
from MyGNode.MyReadParamNode import MyReadParamNode


def tutorial_condition():
    b0, b1, b2 = MyNode1("conditionNodeB0"), MyNode2("conditionNodeB1"), MyNode1("conditionNodeB2")
    b_condition = MyCondition([b0, b1, b2])

    d0, d1, d2 = MyNode1("conditionNodeD0"), MyNode1("conditionNodeD1"), MyNode1("conditionNodeD2")
    d_condition = MyParamCondition([d0, d1, d2])

    pipeline = GPipeline()
    a, c = MyWriteParamNode(), MyReadParamNode()

    pipeline.registerGElement(a, set(), "writeNodeA")
    pipeline.registerGElement(b_condition, {a}, "conditionB")
    pipeline.registerGElement(c, {b_condition}, "readNodeC")
    pipeline.registerGElement(d_condition, {c}, "conditionD")

    pipeline.init()
    for i in range(0, 3):
        status = pipeline.run()
        print("==== tutorial_condition: loop : {0}, and run status = {1}".format(i + 1, status.getCode()))

    pipeline.destroy()


if __name__ == '__main__':
    tutorial_condition()
