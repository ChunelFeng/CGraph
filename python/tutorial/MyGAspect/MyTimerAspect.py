"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyTimerAspect
@Time: 2025/3/5 23:38
@Desc: 
"""

import time

from pycgraph import GAspect, CStatus

class MyTimerAspect(GAspect):
    _start_time = None

    def beginRun(self):
        self._start_time = time.time()
        return CStatus()

    def finishRun(self, curStatus: CStatus):
        span = time.time() - self._start_time
        print('----> [MyTimerAspect] {0} time cost is : {1}s'.format(self.getName(), round(span, 2)))
        return
