"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T00-HelloCGraph
@Time: 2025/2/21 23:25
@Desc: 
"""

from PyCGraph import GNode, GPipeline, CStatus

from MyPyGNode.HelloCGraphPyNode import HelloCGraphPyNode

def tutorial_hello_cgraph():
    pipeline = GPipeline()
    hcg = HelloCGraphPyNode()
    pipeline.registerGElement(hcg)
    pipeline.process()


if __name__ == '__main__':
    tutorial_hello_cgraph()
