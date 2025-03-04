"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T00-HelloCGraph
@Time: 2025/2/21 23:25
@Desc: 
"""

from PyCGraph import GNode, GPipeline

from MyGNode.HelloCGraphNode import HelloCGraphNode


def tutorial_hello_cgraph():
    pipeline = GPipeline()
    hcg = HelloCGraphNode()
    pipeline.registerGElement(hcg)
    pipeline.process()


if __name__ == '__main__':
    tutorial_hello_cgraph()
