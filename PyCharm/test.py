from math import pi,acos,atan2,asin,cos,sin
import math
import numpy as np

#参考：https://blog.csdn.net/qq_39777417/article/details/96133017

RAD2ANG = pi/180

def ANG2RAD(N):
	return N/RAD2ANG

def inverseKinematics(x, y, z):
	l2 = 105
	l3 = 105
	l4 = 50 #3节手臂的长度
	i = 0
	j0 = atan2(y,x)
	a = x / cos(j0)
	if x == 0:
		a = y       #如果x为0，需要交换x，y

	b = z

	for j1 in range (-90,90):
		j1 = j1* RAD2ANG
		temp = (math.pow(a, 2)+math.pow(b, 2)+math.pow(l2, 2)-math.pow(l3, 2)-math.pow(l4,2)-2*a*l2*sin(j1)
				- 2 * b * l2 * cos(j1)) / (2 * l3 * l4)
		if abs(temp) > 1:
			continue
		j3 = acos(temp)
		#if (abs(ANG2RAD(j3)) >= 135) { j1 = ANG2RAD(j1) continue }
		m = l3 * sin(j1) + l4 * sin(j1)*cos(j3) + l4 * cos(j1)*sin(j3)
		n = l3 * cos(j1) + l4 * cos(j1)*cos(j3) - l4 * sin(j1)*sin(j3)
		t = a - l2 * sin(j1)
		p = pow(pow(n, 2) + pow(m, 2), 0.5)
		q = asin(m / p)
		j2 = asin(t / p) - q
		#if (abs(ANG2RAD(j2)) >= 135) { j1 = ANG2RAD(j1) continue }
		#/***************计算正解然后与目标解对比，看解是否正确**************/
		x1 = np.round((l2 * sin(j1) + l3 * sin(j1 + j2) + l4 * sin(j1 + j2 + j3))*cos(j0))
		y1 = np.round((l2 * sin(j1) + l3 * sin(j1 + j2) + l4 * sin(j1 + j2 + j3))*sin(j0))
		z1 = np.round(l2 * cos(j1) + l3 * cos(j1 + j2) + l4 * cos(j1 + j2 + j3))
		j1 = np.round(ANG2RAD(j1),2)
		j2 = np.round(ANG2RAD(j2),2)
		j3 = np.round(ANG2RAD(j3),2)
		if (abs(j1)>90 or abs(j2)>90 or abs(j3)>90 ):
			continue
		if (x1<(x + 2)  and x1 >(x - 2)  and y1<(y + 2)  and y1 >(y - 2)  and z1<(z + 2)  and z1 >(z - 2)):
			print("j1:{},j2:{},j3:{},j4:{},x:{},y:{},z:{}".format(np.round(ANG2RAD(j0)), j1, j2, j3, x1, y1, z1))
			i = 1
	for j1 in range (-90,90)  : #这个循环是为了求解另一组解，j2 = asin(t / p) - qj2 = -(asin(t / p) - q)多了个负号
		j1 *= RAD2ANG
		temp = (math.pow(a, 2) + math.pow(b, 2) + math.pow(l2, 2) - math.pow(l3, 2) - math.pow(l4,2) -
				2 * a * l2 * sin(j1)- 2 * b * l2 * cos(j1)) / (2 * l3 * l4)
		if abs(temp) > 1:
			continue
		j3 = acos(temp)
		#if (abs(ANG2RAD(j3)) >= 135) { j1 = ANG2RAD(j1) continue }
		m = l3 * sin(j1) + l4 * sin(j1)*cos(j3) + l4 * cos(j1)*sin(j3)
		n = l3 * cos(j1) + l4 * cos(j1)*cos(j3) - l4 * sin(j1)*sin(j3)
		t = a - l2 * sin(j1)
		p = pow(pow(n, 2) + pow(m, 2), 0.5)
		q = asin(m / p)
		j2 = -(asin(t / p) - q)
		#if (abs(ANG2RAD(j2)) >= 135) { j1 = ANG2RAD(j1) continue }
		x1 = np.round((l2 * sin(j1) + l3 * sin(j1 + j2) + l4 * sin(j1 + j2 + j3))*cos(j0))
		y1 = np.round((l2 * sin(j1) + l3 * sin(j1 + j2) + l4 * sin(j1 + j2 + j3))*sin(j0))
		z1 = np.round(l2 * cos(j1) + l3 * cos(j1 + j2) + l4 * cos(j1 + j2 + j3))
		j1 = np.round(ANG2RAD(j1),2)
		j2 = np.round(ANG2RAD(j2),2)
		j3 = np.round(ANG2RAD(j3),2)
		if (abs(j1)>90 or abs(j2)>90 or abs(j3)>90 ):
			continue
		if (x1<(x + 2)  and x1 >(x - 2)  and y1<(y + 2)  and y1 >(y - 2)  and z1<(z + 2)  and z1 >(z - 2)):
			print("j1:{},j2:{},j3:{},j4:{},x:{},y:{},z:{}".format(np.round(ANG2RAD(j0)), j1, j2, j3, x1, y1, z1))
			i = 1

	if (i == 0):
		print("无解")


inverseKinematics(20,150,120)
