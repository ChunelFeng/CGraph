"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T06-Condition
@Time: 2025/2/25 23:34
@Desc: 
"""

from PyCGraph import GPipeline, CStatus

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2
from MyGCondition.MyCondition import MyCondition

def tutorial_condition():
    b1, b2, b3 = MyNode1("conditionNodeB0"), MyNode2("conditionNodeB1"), MyNode1("conditionNodeB2", 2)
    b_condition = MyCondition([b1, b2, b3])

    pipeline = GPipeline()
    a, c = MyNode1(), MyNode2()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b_condition, {a}, "conditionB")
    pipeline.registerGElement(c, {b_condition}, "nodeC")

    pipeline.process(4)


if __name__ == '__main__':
    tutorial_condition()
