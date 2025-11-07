"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyConnApsect
@Time: 2025/3/30 21:13
@Desc: 
"""

from pycgraph import GAspect, CStatus

class MyConnAspect(GAspect):
    def __init__(self, ip: str, port: int):
        super().__init__()
        self.ip_ = ip
        self.port_ = port

    def beginRun(self):
        print('    ----> [MyConnAspect] [{0} : {1}] has been connected...'.format(self.ip_, self.port_))
        return CStatus()

    def finishRun(self, curStatus: CStatus):
        if curStatus.isOK():
            print('    <---- [MyConnAspect] [{0} : {1}] has been connected...'.format(self.ip_, self.port_))
        else:
            print('    <---- [MyConnAspect] [{0} : {1}] is already been disconnected...'.format(self.ip_, self.port_))
        return

