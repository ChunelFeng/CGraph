"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyMutable
@Time: 2025/4/9 00:05
@Desc: 
"""

from typing import List

from pycgraph import GMutable, CStatus, GElement

from MyParams.MyParam import MyParam

class MyMutable(GMutable):
    def reshape(self, elements: List[GElement]) -> CStatus:
        param: MyParam = self.getGParamWithNoEmpty('param1')
        cnt = param.count % 4
        if 0 == cnt:
            print("    ---- run as a->[b,c]")
            elements[1].addDependGElements({elements[0]})
            elements[2].addDependGElements({elements[0]})
            elements[0].setVisible(True)    # must set all used elements visible
            elements[1].setVisible(True)
            elements[2].setVisible(True)
        elif 1 == cnt:
            print("    ---- run as c(*3)->b->a")
            elements[0].addDependGElements({elements[1]})
            elements[1].addDependGElements({elements[2]})
            elements[0].setVisible(True)
            elements[1].setVisible(True)
            elements[2].setVisible(True)
            elements[2].setLoop(3)
        elif 2 == cnt:
            print("    ---- run as a->c, do not run b")
            elements[2].addDependGElements({elements[0]})
            elements[0].setVisible(True)
            elements[2].setVisible(True)
        elif 3 == cnt:
            print("    ---- run as [a,b(*2),c]")
            elements[0].setVisible(True)
            elements[1].setVisible(True)
            elements[1].setLoop(2)
            elements[2].setVisible(True)
        return CStatus()
