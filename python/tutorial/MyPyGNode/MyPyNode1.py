"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyPyNode1
@Time: 2025/2/21 23:47
@Desc: 
"""

from datetime import datetime
import time

from PyCGraph import GNode, CStatus

class MyPyNode1(GNode):
    def run(self):
        print("[{0}] {1}, enter MyPyNode1 run function. Sleep for 2 second ... ".format(datetime.now(), self.getName()))
        time.sleep(1)
        return CStatus()
