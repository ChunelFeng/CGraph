"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyNode1
@Time: 2025/2/21 23:47
@Desc: 
"""

from datetime import datetime
import time

from pycgraph import GNode, CStatus

class MyNode1(GNode):
    def run(self):
        print("[{0}] {1}, enter MyNode1 run function. Sleep for 1 second ... ".format(datetime.now(), self.getName()))
        time.sleep(1)
        return CStatus()
