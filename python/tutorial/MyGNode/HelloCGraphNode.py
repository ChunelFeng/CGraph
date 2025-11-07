"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: HelloCGraphPyNode
@Time: 2025/2/21 23:30
@Desc: 
"""

from pycgraph import GNode, CStatus

class HelloCGraphNode(GNode):
    def run(self):
        print("Hello, pycgraph.")
        return CStatus()
