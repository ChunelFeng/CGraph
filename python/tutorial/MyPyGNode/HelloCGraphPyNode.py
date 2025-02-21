"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: HelloCGraphPyNode
@Time: 2025/2/21 23:30
@Desc: 
"""

from PyCGraph import GNode, CStatus

class HelloCGraphPyNode(GNode):
    def run(self):
        print("Hello PyCGraph")
        return CStatus()
