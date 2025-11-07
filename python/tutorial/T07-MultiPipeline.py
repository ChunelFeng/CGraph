"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T07-MultiPipeline
@Time: 2025/4/1 23:37
@Desc: 
"""

from pycgraph import GPipeline, UThreadPoolConfig, UThreadPool

from MyGNode.MyNode1 import MyNode1
from MyGNode.MyNode2 import MyNode2


def async_pipeline_1(pipeline: GPipeline):
    a, b, c, d = MyNode1(), MyNode2(), MyNode1(), MyNode2()

    pipeline.registerGElement(a, set(), "node1A")
    pipeline.registerGElement(b, {a}, "node1B")
    pipeline.registerGElement(c, {a}, "node1C")
    pipeline.registerGElement(d, {a}, "node1D")

    return pipeline.asyncProcess(3)


def async_pipeline_2(pipeline: GPipeline):
    a, b, c = MyNode1(), MyNode2(), MyNode1()

    pipeline.registerGElement(a, set(), "node2A")
    pipeline.registerGElement(b, {a}, "node2B")
    pipeline.registerGElement(c, {b}, "node2C")

    return pipeline.asyncProcess(5)


def tutorial_multi_pipeline():
    pipeline1, pipeline2 = GPipeline(), GPipeline()

    config = UThreadPoolConfig()
    config.default_thread_size = 4
    config.max_thread_size = 4
    config.monitor_enable = False
    pool = UThreadPool(True, config)

    pipeline1.setSharedThreadPool(pool)
    pipeline2.setSharedThreadPool(pool)

    result1 = async_pipeline_1(pipeline1)
    result2 = async_pipeline_2(pipeline2)

    result1.wait()
    result2.wait()


if __name__ == '__main__':
    tutorial_multi_pipeline()
