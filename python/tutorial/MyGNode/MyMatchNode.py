"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyMatchNode
@Time: 2025/3/7 23:54
@Desc: 
"""

import time
from datetime import datetime

from pycgraph import GNode, CStatus

from MyParams.MyParam import MyParam


class MyMatchNode(GNode):
    def run(self):
        time.sleep(1)
        print("[{0}] run MyMatchNode: {1}".format(datetime.now(), self.getName()))

        return CStatus()

    def isMatch(self):
        param: MyParam = self.getGParamWithNoEmpty('param1')
        result = 0 != param.count % 2
        if result:
            print('    match, ready to run {0}'.format(self.getName()))
        else:
            print('    not match, will not run {0}'.format(self.getName()))
        return result
