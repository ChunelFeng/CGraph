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
        return ""

    def run(self):
        print("[{0}] {1}, enter MyNode2 run function. Sleep for 2 second ... ".format(datetime.now(), self.getName()))
        time.sleep(2)
        # support direct return 0 or "" as ok, after v3.3.0
        # return -1 (<0) or "xxxx" as error
        return 0

    def destroy(self):
        print("[DESTROY] [{0}], enter MyNode2 destroy function.".format(self.getName()))
        return CStatus()
