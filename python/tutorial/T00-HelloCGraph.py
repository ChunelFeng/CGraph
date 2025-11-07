"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T00-HelloCGraph
@Time: 2025/2/21 23:25
@Desc: 
"""

from pycgraph import GPipeline

from MyGNode.HelloCGraphNode import HelloCGraphNode


def tutorial_hello_cgraph():
    pipeline = GPipeline()
    pipeline.registerGElement(HelloCGraphNode())
    pipeline.process()


if __name__ == '__main__':
    tutorial_hello_cgraph()
