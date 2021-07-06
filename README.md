### 软件以及文件夹说明

#### 功能描述

​		一款基于Python语言人机交互界面应用。该应用的界面框架选择的是PYQT5，界面设计工具选择的是QtDesigner，本应用的其他Python功能模块有JSON、Requests、Matplotlib、Serial等。该应用界面能实现与下位机进行网络、串口通信，将数据打包成JSON格式等功能，可以实时显示下位机的网络在线状态以及下位机串口输出的日志内容，用户能够通过上位机软件进行每一个自由度和机械爪子的控制，实现WiFi的账号密码设置、开机演示的动作设置以及存储动作组等功能。

#### KeilProject ：

​		下位机程序代码，工程文件于 ./KeilProject/MDK-ARM/GraduationProject.uvprojx ，工程运行软件为Keil5 μVision IDE



#### PyCharm：

​		详细说明位于 ./PyCharm/Readme.md

​		工程运行软件为 JetBrains PyCharm ，环境为Python 3.9



#### packProject：

​		详细说明位于 ./packProject/Readme.md

​		工程运行软件为Inno Setup Compiler，但不依赖该工程软件，可直接运行 ./packProject/机械臂上位机控制程序安装包.exe，依赖CH340驱动，位于 ./packProject/CH340驱动(USB串口驱动)_XP_WIN7共用.zip





