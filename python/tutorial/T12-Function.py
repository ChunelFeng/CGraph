"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T12-Function
@Time: 2025/4/13 18:46
@Desc: 
"""

import time

from pycgraph import GPipeline, GFunction, CFunctionType, CStatus

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyWriteParamNode import MyWriteParamNode
from MyParams.MyParam import MyParam


def c_function_run():
    print('enter c_function_run, sleep for 1s')
    time.sleep(1)
    return CStatus()

def d_function_init(d: GFunction):
    print('[{0}] do init function....'.format(d.getName()))
    return CStatus()

def d_function_run(d: GFunction, num: int):
    # get GParam info in GFunction
    param: MyParam = d.getGParamWithNoEmpty('param1')
    param.count += num
    print('[{0}] do run function, count is {1}, value is {2}'.format(
        d.getName(), param.count, param.value))
    return CStatus()


def tutorial_function():
    pipeline = GPipeline()
    a, b = MyNode1(), MyWriteParamNode()
    c_function, d_function = GFunction(), GFunction()

    c_function.setFunction(CFunctionType.RUN, c_function_run)

    d_function.setFunction(CFunctionType.INIT, lambda: d_function_init(d_function))
    d_function.setFunction(CFunctionType.RUN, lambda: d_function_run(d_function, 10))

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c_function, {b}, "functionC")
    pipeline.registerGElement(d_function, {c_function}, "nodeD")

    pipeline.process()


if __name__ == '__main__':
    tutorial_function()
