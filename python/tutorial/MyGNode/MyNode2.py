"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyPyNode2
@Time: 2025/2/21 23:48
@Desc: 
"""

from datetime import datetime
import time

from pycgraph import GNode, CStatus

class MyNode2(GNode):
    def init(self):
        print("[INIT] [{0}], enter MyNode2 init function.".format(self.getName()))
        return CStatus()

    def run(self):
        print("[{0}] {1}, enter MyNode2 run function. Sleep for 2 second ... ".format(datetime.now(), self.getName()))
        time.sleep(2)
        return CStatus()

    def destroy(self):
        print("[DESTROY] [{0}], enter MyNode2 destroy function.".format(self.getName()))
        return CStatus()
