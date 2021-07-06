from mpl_toolkits.mplot3d import Axes3D
import matplotlib
matplotlib.use("Qt5Agg")  # 声明使用QT5
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
from matplotlib import cm

import numpy as np
from math import radians, pi, acos, atan2, asin, cos, sin
import math

from PyQt5.QtWidgets import QGridLayout


#####################################
#  参考：https://blog.csdn.net/hehedadaq/article/details/106878642
#  六自由度机械臂的运动学分析和轨迹规划研究
#           l2      l3        l4
#       o--------o----------o----┐
#       |                     \  | l5
#    l1 |                      \ |
#       |                        o
#      [|]
#       ▲
#       机械臂示意图 需要修改臂长
#
####################################

l1 = 30
l2 = 105
l3 = 105
l4 = 40
l5 = -30

RAD2ANG = pi / 180


def ANG2RAD(N):
    return N / RAD2ANG

#创建一个matplotlib图形绘制类
class MyFigure(FigureCanvas):
    def __init__(self,width=5, height=4, dpi=100):
        #第一步：创建一个创建Figure
        self.fig = Figure(figsize=(width, height), dpi=dpi)
        #第二步：在父类中激活Figure窗口
        super(MyFigure,self).__init__(self.fig) #此句必不可少，否则不能显示图形
        #第三步：创建一个子图，用于绘制图形用，111表示子图编号，如matlab的subplot(1,1,1)
        #self.axes = self.fig.add_subplot(111)
    #第四步：就是画图，【可以在此类中画，也可以在其它类中画】
# ————————————————
# 版权声明：本文为CSDN博主「忍耐恒_至拙胜至巧」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
# 原文链接：https://blog.csdn.net/panrenlong/article/details/80183519

class calculatingDH():
    def __init__(self,ui):
        self.ui=ui
        ui.pushButton_8.clicked.connect(lambda:self.refresh())
        np.set_printoptions(suppress=True,)
        self.θ1 = 180 / 2
        self.θ2 = -180 / 2
        self.θ3 = 180 / 2
        self.θ4 = 180 / 2
        self.θ5 = -180 / 4

        # 第五步：定义MyFigure类的一个实例
        self.F1 = MyFigure(width=450, height=500, dpi=80)

        self.F2 = MyFigure(width=450, height=500, dpi=80)
        self.plt1 = self.F1.fig.add_subplot(111)
        self.plt1.cla()
        self.plt2 = self.F2.fig.gca(projection='3d')
        self.plt2.clear()
        # 第六步：在GUI的groupBox中创建一个布局，用于添加MyFigure类的实例（即图形）后其他部件。
        self.gridlayout = QGridLayout(ui.widget)  # 继承容器groupBox
        self.gridlayout.setContentsMargins(1,1,1,1)
        self.gridlayout.addWidget(self.F1,0,0)
        self.gridlayout.addWidget(self.F2, 1, 0)

        self.DH_Array = [[0, 0, 0, self.θ1],
                         [90, l1, 0, self.θ2],
                         [0, l2, 0, self.θ3],
                         [0, l3, 0, self.θ4],
                         [-90, l4, l5, self.θ5]]
        self.XYZOutput = []

        self.draw2D()
        ui.lineEdit_31.setText(str(self.XYZOutput[-1][0]))
        ui.lineEdit_29.setText(str(self.XYZOutput[-1][2]))
        ui.lineEdit_30.setText(str(self.XYZOutput[-1][1]))

    def refresh(self):
        self.plt1.cla()
        self.plt2.clear()
        self.draw2D()
        self.F1.fig.canvas.draw()
        self.F1.fig.canvas.flush_events()
        self.F2.fig.canvas.draw()
        self.F2.fig.canvas.flush_events()
        self.ui.lineEdit_31.setText(str(self.XYZOutput[-1][0]))
        self.ui.lineEdit_29.setText(str(self.XYZOutput[-1][2]))
        self.ui.lineEdit_30.setText(str(self.XYZOutput[-1][1]))

    def draw2D(self):
        self.calcXYZ()
        xlist = []
        zlist = []
        ylist = []
        for item in self.XYZOutput:
            ylist.append(item[0])
            zlist.append(item[1])
            xlist.append(item[2])
        # print(xlist,ylist,zlist)
        self.plt1.axis([-260,260,0,260])
        self.plt1.plot(xlist[0], zlist[0], 'bs',markersize=8)
        self.plt1.plot(xlist[1:],zlist[1:],'bo',markersize=8)
        self.plt1.plot(xlist, zlist,color="red",linewidth=3)
        x1=xlist[-2]
        z1=zlist[-2]
        x2 = xlist[-1]
        z2 = zlist[-1]
        x3 = l4 * cos(atan2((z2 - z1) , (x2 - x1)) - atan2(abs(l5) , abs(l4))) + x1
        z3 = l4 * sin(atan2((z2 - z1) , (x2 - x1))- atan2(abs(l5) , abs(l4))) + z1
        self.plt1.plot([x1,x3], [z1,z3], 'r--', linewidth=1)
        self.plt1.plot([x2, x3], [z2, z3], 'r--', linewidth=1)
        self.plt1.plot([x2, x2+x3-x1], [z2, z2+z3-z1], 'g',linewidth=3)


        self.plt2.bar3d(-260, -260, 0, 500, 500, 260, color="green",zsort='average',edgecolor='green',linewidth=0.5,alpha=0)
        self.plt2.plot(xlist,ylist,zlist,linewidth=3)
        self.plt2.plot(xlist[0],ylist[0], zlist[0], 'bs', markersize=4)
        self.plt2.plot(xlist[1:],ylist[1:], zlist[1:], 'bo', markersize=4)




    def dh_matrix(self, alpha, a, d, theta):
        # 传入四个DH参数，根据公式3-6，输出一个T矩阵。
        alpha = alpha / 180 * np.pi
        theta = theta / 180 * np.pi
        matrix = np.identity(4)
        matrix[0, 0] = cos(theta)
        matrix[0, 1] = -sin(theta)
        matrix[0, 2] = 0
        matrix[0, 3] = a
        matrix[1, 0] = sin(theta) * cos(alpha) if (abs(sin(theta) * cos(alpha)) > 1e-16) else 0
        matrix[1, 1] = cos(theta) * cos(alpha) if (abs(cos(theta) * cos(alpha)) > 1e-16) else 0
        matrix[1, 2] = -sin(alpha) if (abs(sin(alpha)) > 1e-16) else 0
        matrix[1, 3] = -sin(alpha) * d if (abs(sin(alpha)) > 1e-16) else 0
        matrix[2, 0] = sin(theta) * sin(alpha) if (abs(sin(theta) * sin(alpha)) > 1e-16) else 0
        matrix[2, 1] = cos(theta) * sin(alpha) if (abs(cos(theta) * sin(alpha)) > 1e-16) else 0
        matrix[2, 2] = cos(alpha) if (abs(cos(alpha)) > 1e-16) else 0
        matrix[2, 3] = cos(alpha) * d if (abs(cos(alpha)) > 1e-16) else 0
        matrix[3, 0] = 0
        matrix[3, 1] = 0
        matrix[3, 2] = 0
        matrix[3, 3] = 1
        return matrix
        # ————————————————
        # 版权声明：本文为CSDN博主「hehedadaq」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
        # 原文链接：https://blog.csdn.net/hehedadaq/article/details/106878642

    def calcXYZ(self):
        self.refreshAngle()
        jointHm = []
        for i in self.DH_Array:
            jointHm.append(self.dh_matrix(i[0], i[1], i[2], i[3]))
        # for i in jointHm:
        #     print(i)
        jointCalcResult = []
        jointCalcResult.append(jointHm[0])
        for i in range(len(jointHm)-1):
            jointCalcResult.append(np.dot(jointCalcResult[i], jointHm[i + 1]))
        # for i in jointCalcResult:
        #     print(i)
        self.XYZOutput = []
        for item in jointCalcResult:
            # print(np.round(item[:3, 3]))
            self.XYZOutput.append(np.round(item[:3, 3]))

    def refreshAngle(self):
        self.θ1 = eval(self.ui.lineEdit.text())
        self.θ2 = 180-eval(self.ui.lineEdit_4.text())
        self.θ3 = eval(self.ui.lineEdit_12.text())-90
        self.θ4 = 180-eval(self.ui.lineEdit_9.text())-90
        self.θ5 = eval(self.ui.lineEdit_16.text())-90
        self.DH_Array = [[-90, 0, 0, 00],
                         [0, 0, l1, self.θ1],
                         [90,0, 0, self.θ2],
                         [0, l2, 0, self.θ3],
                         [0, l3, 0, self.θ4],
                         [-90, l4, l5,self.θ5]]
        print(self.DH_Array)

    def inverseKinematics(self, x, y, z):
        hl4 = pow(l4**2+l5**2,0.5)  # 3节手臂的长度
        i = 0
        j0 = atan2(y, x)
        a = x / cos(j0)
        if x == 0:
            a = y  # 如果x为0，需要交换x，y

        b = z

        calcResult = []

        for j1 in range(-90, 90):
            j1 = j1 * RAD2ANG
            temp = (math.pow(a, 2) + math.pow(b, 2) + math.pow(l2, 2) - math.pow(l3, 2)
                    - math.pow(hl4, 2) - 2 * a * l2 * sin(j1) - 2 * b * l2 * cos(j1)) / (2 * l3 * hl4)
            if abs(temp) > 1:
                continue
            j3 = acos(temp)
            # if (abs(ANG2RAD(j3)) >= 135) { j1 = ANG2RAD(j1) continue }
            m = l3 * sin(j1) + hl4 * sin(j1) * cos(j3) + hl4 * cos(j1) * sin(j3)
            n = l3 * cos(j1) + hl4 * cos(j1) * cos(j3) - hl4 * sin(j1) * sin(j3)
            t = a - l2 * sin(j1)
            p = pow(pow(n, 2) + pow(m, 2), 0.5)
            q = asin(m / p)
            j2 = asin(t / p) - q
            # if (abs(ANG2RAD(j2)) >= 135) { j1 = ANG2RAD(j1) continue }
            # /***************计算正解然后与目标解对比，看解是否正确**************/
            x1 = np.round((l2 * sin(j1) + l3 * sin(j1 + j2) + hl4 * sin(j1 + j2 + j3)) * cos(j0))
            y1 = np.round((l2 * sin(j1) + l3 * sin(j1 + j2) + hl4 * sin(j1 + j2 + j3)) * sin(j0))
            z1 = np.round(l2 * cos(j1) + l3 * cos(j1 + j2) + hl4 * cos(j1 + j2 + j3))
            j1 = np.round(ANG2RAD(j1), 2)
            j2 = np.round(ANG2RAD(j2), 2)
            j3 = np.round(ANG2RAD(j3), 2)
            if (abs(j1) > 90 or abs(j2) > 90 or abs(j3+ANG2RAD(acos(l4/hl4))) > 90):
                continue
            if (x1 < (x + 2) and x1 > (x - 2) and y1 < (y + 2) and y1 > (y - 2) and z1 < (z + 2) and z1 > (z - 2)):
                calcResult.append([np.round(ANG2RAD(j0)),j1,j2,j3])
                print("j1:{},j2:{},j3:{},j4:{},x:{},y:{},z:{}".format(np.round(ANG2RAD(j0)), j1, j2, j3, x1, y1, z1))
                i = 1
        for j1 in range(-90, 90):  # 这个循环是为了求解另一组解，j2 = asin(t / p) - qj2 = -(asin(t / p) - q)多了个负号
            j1 *= RAD2ANG
            temp = (math.pow(a, 2) + math.pow(b, 2) + math.pow(l2, 2) - math.pow(l3, 2) - math.pow(hl4, 2) -
                    2 * a * l2 * sin(j1) - 2 * b * l2 * cos(j1)) / (2 * l3 * hl4)
            if abs(temp) > 1:
                continue
            j3 = acos(temp)
            # if (abs(ANG2RAD(j3)) >= 135) { j1 = ANG2RAD(j1) continue }
            m = l3 * sin(j1) + hl4 * sin(j1) * cos(j3) + hl4 * cos(j1) * sin(j3)
            n = l3 * cos(j1) + hl4 * cos(j1) * cos(j3) - hl4 * sin(j1) * sin(j3)
            t = a - l2 * sin(j1)
            p = pow(pow(n, 2) + pow(m, 2), 0.5)
            q = asin(m / p)
            j2 = -(asin(t / p) - q)
            # if (abs(ANG2RAD(j2)) >= 135) { j1 = ANG2RAD(j1) continue }
            x1 = np.round((l2 * sin(j1) + l3 * sin(j1 + j2) + hl4 * sin(j1 + j2 + j3)) * cos(j0))
            y1 = np.round((l2 * sin(j1) + l3 * sin(j1 + j2) + hl4 * sin(j1 + j2 + j3)) * sin(j0))
            z1 = np.round(l2 * cos(j1) + l3 * cos(j1 + j2) + hl4 * cos(j1 + j2 + j3))
            j1 = np.round(ANG2RAD(j1), 2)
            j2 = np.round(ANG2RAD(j2), 2)
            j3 = np.round(ANG2RAD(j3), 2)
            if (abs(j1) > 90 or abs(j2) > 90 or abs(j3+ANG2RAD(acos(l4/hl4))) > 90):
                continue
            if (x1 < (x + 2) and x1 > (x - 2) and y1 < (y + 2) and y1 > (y - 2) and z1 < (z + 2) and z1 > (z - 2)):
                calcResult.append([np.round(ANG2RAD(j0)), j1, j2, j3])
                print("j1:{},j2:{},j3:{},j4:{},x:{},y:{},z:{}".format(np.round(ANG2RAD(j0)), j1, j2, j3, x1, y1, z1))
                i = 1

        if (i == 0):
            print("无解")
            calcResult = False
        return calcResult

    def calcAngle(self):
        hl4 = pow(l4 ** 2 + l5 ** 2, 0.5)  # 3节手臂的长度
        x = eval(self.ui.lineEdit_31.text())
        y = eval(self.ui.lineEdit_29.text())
        z = eval(self.ui.lineEdit_30.text())
        calcResult = self.inverseKinematics(x,y,z)
        if calcResult != False:
            j0 = calcResult[0][0]
            j1 = calcResult[0][1] + 90
            j2 = calcResult[0][2] + 90
            j3 = calcResult[0][3]+ANG2RAD(acos(l4/hl4)) + 90
            self.ui.lineEdit.setText(str(j0))
            self.ui.lineEdit_4.setText(str(j1))
            self.ui.lineEdit_12.setText(str(j2))
            self.ui.lineEdit_9.setText(str(j3))


        # clc = calculatingDH()
# clc.calcXYZ()
