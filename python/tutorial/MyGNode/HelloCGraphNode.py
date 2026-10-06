"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: HelloCGraphPyNode
@Time: 2025/2/21 23:30
@Desc: 
"""

from pycgraph import GNode, CStatus, node
@node
class HelloCGraphNode:
    def run(self):
        print("Hello, pycgraph.")
        return CStatus()
