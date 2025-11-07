"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyMonitorDaemon
@Time: 2025/3/30 00:49
@Desc: 
"""

from pycgraph import GDaemon

from MyParams.MyParam import MyParam

class MyMonitorGDaemon(GDaemon):
    def daemonTask(self, _):
        param: MyParam = self.getGParamWithNoEmpty('param1')
        print("----> [MyMonitorDaemon] monitor span is [{0}] ms, count is [{1}]".format(
            self.getInterval(), param.count))
