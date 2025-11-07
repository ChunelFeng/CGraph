"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyEventNode
@Time: 2025/3/22 01:46
@Desc: 
"""

from datetime import datetime

from pycgraph import GNode, CStatus, GEventType

class MyEventNode(GNode):
    def run(self):
        print('[{0}] before event notify, {1}'.format(datetime.now(), self.getName()))
        self.notify('event1', GEventType.SYNC)
        print('[{0}] after event notify, {1}'.format(datetime.now(), self.getName()))
        return CStatus()
