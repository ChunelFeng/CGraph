"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyPrintEvent
@Time: 2025/3/22 00:37
@Desc: 
"""

import time

from pycgraph import GEvent

class MyPrintEvent(GEvent):
    _times: int = 0

    def trigger(self, _):
        time.sleep(1)
        p1 = self.getGParamWithNoEmpty('param1')
        self._times += 1
        print('    ----> trigger {0} times, val = {1}'.format( self._times, p1.value))
