import sys, os
from PyQt5.QtWidgets import QApplication, QMainWindow,QDialog
from PyQt5 import QtCore
from PyQt5.QtGui import QPixmap,QPalette,QColor
from PyQt5.QtCore import QThread
import time,pickle

import uidesign
import child_dialog
import actGroup
import graduation
import changeWiFi
import serialControl
import win32ui
import InputGroupNum
import saveToGroup
import DH_Calc

global serialCB,DH_Calcing

class RefreshInternet(QThread):
    #自定义信号
    def __init__(self,ui):
        self.ui=ui
        super().__init__()

    def run(self):
        while True:
            networkStatus = graduation.initOneNET()
            if networkStatus == "online":
                ui.label_2.setText(QtCore.QCoreApplication.translate("MainWindow", "在线"))
                ui.label_2.setStyleSheet("background-color:  Lime ")
                ui.label_43.setText("")
                ui.radioButton_2.setEnabled(True)
            elif networkStatus == False:
                ui.label_2.setText(QtCore.QCoreApplication.translate("MainWindow", "离线"))
                ui.label_2.setStyleSheet("background-color: FloralWhite  ")
                ui.label_43.setText(QtCore.QCoreApplication.translate("MainWindow", "无网络连接"))
                ui.radioButton.setChecked(True)
                ui.radioButton_2.setEnabled(False)
                ui.radioButton.setChecked(True)
            else:
                ui.label_2.setText(QtCore.QCoreApplication.translate("MainWindow", "离线"))
                ui.label_2.setStyleSheet("background-color: FloralWhite  ")
                ui.radioButton_2.setEnabled(False)
                ui.radioButton.setChecked(True)

            self.sleep(5)



def ui_buttomInit(ui1 = uidesign.Ui_MainWindow()):
    _translate = QtCore.QCoreApplication.translate
    # 插入主题图标
    ui1.label_19.setStyleSheet("background-color: #253d6a")
    ui1.label_19.setPixmap(QPixmap("./icons/BUCT_logo.png").scaled(190, 52))
    ui1.label_20.setStyleSheet("background-color: #253d6a;color:Azure ")
    ui1.label_21.setPixmap(QPixmap("./icons/OneNet_logo.png").scaled(140,28))

    ui1.pushButton_5.setEnabled(False)
    ui1.radioButton_3.setEnabled(False)
    ui1.radioButton_4.setEnabled(False)
    ui1.radioButton_3.setChecked(True)
    # # 串口检测按钮
    # ui1.pushButton_4.clicked.connect(lambda: serialControl.port_check(ui1))
    #导出按钮
    ui1.pushButton_14.clicked.connect(lambda :saveOutput(ui1))
    #设置波特率表格
    ui1.comboBox_2.addItem("115200")
    ui1.comboBox_2.addItem("1500000")
    ui1.comboBox_2.addItem("2400")
    ui1.comboBox_2.addItem("4800")
    ui1.comboBox_2.addItem("9600")
    ui1.comboBox_2.addItem("14400")
    ui1.comboBox_2.addItem("19200")
    ui1.comboBox_2.addItem("38400")
    ui1.comboBox_2.addItem("57600")
    ui1.comboBox_2.addItem("76800")
    ui1.comboBox_2.addItem("12800")
    ui1.comboBox_2.addItem("230400")
    ui1.comboBox_2.addItem("460800")

    ui1.lineEdit.setText(str((actGroup.act_group_dic1["S1"]["a"]-500)*180/2000))
    ui1.lineEdit_2.setText(str(actGroup.act_group_dic1["S1"]["t"]))
    ui1.lineEdit_3.setText(str(actGroup.act_group_dic1["S2"]["t"]))
    ui1.lineEdit_4.setText(str((actGroup.act_group_dic1["S2"]["a"]-500)*180/2000))
    ui1.lineEdit_12.setText(str((actGroup.act_group_dic1["S3"]["a"]-500)*180/2000))
    ui1.lineEdit_10.setText(str(actGroup.act_group_dic1["S3"]["t"]))
    ui1.lineEdit_9.setText(str((actGroup.act_group_dic1["S4"]["a"]-500)*180/2000))
    ui1.lineEdit_11.setText(str(actGroup.act_group_dic1["S4"]["t"]))
    ui1.lineEdit_16.setText(str((actGroup.act_group_dic1["S5"]["a"]-500)*180/2000))
    ui1.lineEdit_14.setText(str(actGroup.act_group_dic1["S5"]["t"]))
    ui1.lineEdit_15.setText(str(actGroup.act_group_dic1["S1"]["t"]))

    #重置按钮
    ui1.pushButton_3.clicked.connect(lambda :resetButtom(ui1))
    #统一时间文本框
    ui1.lineEdit_15.textChanged.connect(lambda :textChangeHandel(ui1))


    #使能开机演示按钮
    ui1.checkBox.clicked.connect(lambda :enableBootDemo(ui1))

    ui1.tabWidget.currentIndex()
    ui1.pushButton_15.setEnabled(False)
    ui1.pushButton_16.setEnabled(False)

    #设置log窗口格式
    ui1.textBrowser.setStyleSheet("color:Blue ")

def textChangeHandel(ui1 = uidesign.Ui_MainWindow()):
    ui1.lineEdit_2.setText(ui1.lineEdit_15.text())
    ui1.lineEdit_3.setText(ui1.lineEdit_15.text())
    ui1.lineEdit_10.setText(ui1.lineEdit_15.text())
    ui1.lineEdit_11.setText(ui1.lineEdit_15.text())
    ui1.lineEdit_14.setText(ui1.lineEdit_15.text())

def resetButtom(ui1 = uidesign.Ui_MainWindow()):
    if  ui1.tabWidget.currentIndex() == 0 :
        ui1.lineEdit.setText(str((actGroup.act_group_dic1["S1"]["a"] - 500) * 180 / 2000))
        ui1.lineEdit_2.setText(str(actGroup.act_group_dic1["S1"]["t"]))
        ui1.lineEdit_3.setText(str(actGroup.act_group_dic1["S2"]["t"]))
        ui1.lineEdit_4.setText(str((actGroup.act_group_dic1["S2"]["a"] - 500) * 180 / 2000))
        ui1.lineEdit_12.setText(str((actGroup.act_group_dic1["S3"]["a"] - 500) * 180 / 2000))
        ui1.lineEdit_10.setText(str(actGroup.act_group_dic1["S3"]["t"]))
        ui1.lineEdit_9.setText(str((actGroup.act_group_dic1["S4"]["a"] - 500) * 180 / 2000))
        ui1.lineEdit_11.setText(str(actGroup.act_group_dic1["S4"]["t"]))
        ui1.lineEdit_16.setText(str((actGroup.act_group_dic1["S5"]["a"] - 500) * 180 / 2000))
        ui1.lineEdit_14.setText(str(actGroup.act_group_dic1["S5"]["t"]))
        ui1.lineEdit_15.setText(str(actGroup.act_group_dic1["S1"]["t"]))

    elif ui1.tabWidget.currentIndex() == 1 :
        ui1.lineEdit_20.setText("")
        ui1.lineEdit_21.setText("")
        ui1.lineEdit_22.setText("")
        ui1.lineEdit_23.setText("")
        ui1.lineEdit_24.setText("")
        ui1.lineEdit_36.setText("")
        ui1.lineEdit_37.setText("")
        ui1.lineEdit_38.setText("")
        ui1.lineEdit_39.setText("")
        ui1.lineEdit_40.setText("")

        ui1.lineEdit_41.setText("")
        ui1.lineEdit_42.setText("")
        ui1.lineEdit_43.setText("")
        ui1.lineEdit_44.setText("")
        ui1.lineEdit_45.setText("")
        ui1.lineEdit_46.setText("")
        ui1.lineEdit_47.setText("")
        ui1.lineEdit_48.setText("")
        ui1.lineEdit_49.setText("")
        ui1.lineEdit_50.setText("")

        ui1.lineEdit_51.setText("")
        ui1.lineEdit_52.setText("")
        ui1.lineEdit_53.setText("")
        ui1.lineEdit_54.setText("")
        ui1.lineEdit_55.setText("")
        ui1.lineEdit_56.setText("")
        ui1.lineEdit_57.setText("")
        ui1.lineEdit_58.setText("")
        ui1.lineEdit_59.setText("")
        ui1.lineEdit_60.setText("")

        ui1.lineEdit_61.setText("")
        ui1.lineEdit_62.setText("")
        ui1.lineEdit_63.setText("")
        ui1.lineEdit_64.setText("")
        ui1.lineEdit_65.setText("")
        ui1.lineEdit_66.setText("")
        ui1.lineEdit_67.setText("")
        ui1.lineEdit_68.setText("")
        ui1.lineEdit_69.setText("")
        ui1.lineEdit_70.setText("")

        ui1.lineEdit_71.setText("")
        ui1.lineEdit_72.setText("")
        ui1.lineEdit_73.setText("")
        ui1.lineEdit_74.setText("")
        ui1.lineEdit_75.setText("")
        ui1.lineEdit_76.setText("")
        ui1.lineEdit_77.setText("")
        ui1.lineEdit_78.setText("")
        ui1.lineEdit_79.setText("")
        ui1.lineEdit_80.setText("")

        ui1.lineEdit_81.setText("")
        ui1.lineEdit_82.setText("")
        ui1.lineEdit_83.setText("")
        ui1.lineEdit_84.setText("")
        ui1.lineEdit_85.setText("")
        ui1.lineEdit_86.setText("")
        ui1.lineEdit_87.setText("")
        ui1.lineEdit_88.setText("")
        ui1.lineEdit_89.setText("")
        ui1.lineEdit_90.setText("")

        ui1.lineEdit_91.setText("")
        ui1.lineEdit_92.setText("")
        ui1.lineEdit_93.setText("")
        ui1.lineEdit_94.setText("")
        ui1.lineEdit_95.setText("")
        ui1.lineEdit_96.setText("")
        ui1.lineEdit_97.setText("")
        ui1.lineEdit_98.setText("")
        ui1.lineEdit_99.setText("")
        ui1.lineEdit_100.setText("")

        ui1.lineEdit_101.setText("")
        ui1.lineEdit_102.setText("")
        ui1.lineEdit_103.setText("")
        ui1.lineEdit_104.setText("")
        ui1.lineEdit_105.setText("")
        ui1.lineEdit_106.setText("")
        ui1.lineEdit_107.setText("")
        ui1.lineEdit_108.setText("")
        ui1.lineEdit_109.setText("")
        ui1.lineEdit_110.setText("")

def filename():
    dlg = win32ui.CreateFileDialog(1)
    dlg.SetOFNInitialDir('C:/')
    dlg.DoModal()
    filename = dlg.GetPathName()
    filename = filename.replace('\\', '/')
    return filename

def saveOutput(ui1 = uidesign.Ui_MainWindow()):
    FileName = filename()       #+"/logout"+nowtime.strftime("%c").replace(" ","_")+".txt"
    #print(FileName)
    if FileName =="":
        return
    fin = open(FileName,"w")
    fin.write(ui1.textBrowser.toPlainText())
    fin.close()

def enableBootDemo(ui1 = uidesign.Ui_MainWindow()):
    if ui1.checkBox.isChecked():
        ui1.radioButton_3.setEnabled(True)
        ui1.radioButton_4.setEnabled(True)
    else:
        ui1.radioButton_3.setEnabled(False)
        ui1.radioButton_4.setEnabled(False)

def saveGroupToFlash(ui1 = uidesign.Ui_MainWindow()):
    if (ui1.lineEdit_21.text() != "") and (ui1.lineEdit_22.text() != "") and\
            (ui1.lineEdit_23.text() != "") and (ui1.lineEdit_24.text() != "")\
            and(ui1.lineEdit_20.text() != ""):
        S1_a = int(eval(ui1.lineEdit_20.text()) * 2000 / 180 + 500)
        S2_a = int(eval(ui1.lineEdit_21.text()) * 2000 / 180 + 500)
        S3_a = int(eval(ui1.lineEdit_22.text()) * 2000 / 180 + 500)
        S4_a = int(eval(ui1.lineEdit_23.text()) * 2000 / 180 + 500)
        S5_a = int(eval(ui1.lineEdit_24.text()) * 2000 / 180 + 500)

        S1_t = eval(ui1.lineEdit_36.text())
        S2_t = eval(ui1.lineEdit_37.text())
        S3_t = eval(ui1.lineEdit_38.text())
        S4_t = eval(ui1.lineEdit_39.text())
        S5_t = eval(ui1.lineEdit_40.text())
        strtemp="*#10241#*\r\nG1:S1:{} t{} ,S2:{} t{} ,S3:{} t{} ,S4:{} t{} ,S5:{} t{} ".format(S1_a,S1_t,S2_a,S2_t,
                                                                                                 S3_a,S3_t,S4_a,S4_t,
                                                                                                 S5_a,S5_t)
        serialCB.data_send(strtemp)
        time.sleep(0.1)

    if (ui1.lineEdit_41.text() != "") and (ui1.lineEdit_42.text() != "") and\
            (ui1.lineEdit_43.text() != "") and (ui1.lineEdit_44.text() != "")\
            and(ui1.lineEdit_45.text() != ""):
        S1_a = int(eval(ui1.lineEdit_41.text()) * 2000 / 180 + 500)
        S2_a = int(eval(ui1.lineEdit_42.text()) * 2000 / 180 + 500)
        S3_a = int(eval(ui1.lineEdit_43.text()) * 2000 / 180 + 500)
        S4_a = int(eval(ui1.lineEdit_44.text()) * 2000 / 180 + 500)
        S5_a = int(eval(ui1.lineEdit_45.text()) * 2000 / 180 + 500)

        S1_t = eval(ui1.lineEdit_46.text())
        S2_t = eval(ui1.lineEdit_47.text())
        S3_t = eval(ui1.lineEdit_48.text())
        S4_t = eval(ui1.lineEdit_49.text())
        S5_t = eval(ui1.lineEdit_50.text())
        strtemp="*#10241#*\r\nG2:S1:{} t{} ,S2:{} t{} ,S3:{} t{} ,S4:{} t{} ,S5:{} t{} ".format(S1_a,S1_t,S2_a,S2_t,
                                                                                                 S3_a,S3_t,S4_a,S4_t,
                                                                                                 S5_a,S5_t)
        serialCB.data_send(strtemp)
        time.sleep(0.1)

    if (ui1.lineEdit_51.text() != "") and (ui1.lineEdit_52.text() != "") and\
            (ui1.lineEdit_53.text() != "") and (ui1.lineEdit_54.text() != "")\
            and(ui1.lineEdit_55.text() != ""):
        S1_a = int(eval(ui1.lineEdit_51.text()) * 2000 / 180 + 500)
        S2_a = int(eval(ui1.lineEdit_52.text()) * 2000 / 180 + 500)
        S3_a = int(eval(ui1.lineEdit_53.text()) * 2000 / 180 + 500)
        S4_a = int(eval(ui1.lineEdit_54.text()) * 2000 / 180 + 500)
        S5_a = int(eval(ui1.lineEdit_55.text()) * 2000 / 180 + 500)

        S1_t = eval(ui1.lineEdit_56.text())
        S2_t = eval(ui1.lineEdit_57.text())
        S3_t = eval(ui1.lineEdit_58.text())
        S4_t = eval(ui1.lineEdit_59.text())
        S5_t = eval(ui1.lineEdit_60.text())
        strtemp="*#10241#*\r\nG3:S1:{} t{} ,S2:{} t{} ,S3:{} t{} ,S4:{} t{} ,S5:{} t{} ".format(S1_a,S1_t,S2_a,S2_t,
                                                                                                 S3_a,S3_t,S4_a,S4_t,
                                                                                                 S5_a,S5_t)
        serialCB.data_send(strtemp)
        time.sleep(0.1)

    if (ui1.lineEdit_61.text() != "") and (ui1.lineEdit_62.text() != "") and\
            (ui1.lineEdit_63.text() != "") and (ui1.lineEdit_64.text() != "")\
            and(ui1.lineEdit_65.text() != ""):
        S1_a = int(eval(ui1.lineEdit_61.text()) * 2000 / 180 + 500)
        S2_a = int(eval(ui1.lineEdit_62.text()) * 2000 / 180 + 500)
        S3_a = int(eval(ui1.lineEdit_63.text()) * 2000 / 180 + 500)
        S4_a = int(eval(ui1.lineEdit_64.text()) * 2000 / 180 + 500)
        S5_a = int(eval(ui1.lineEdit_65.text()) * 2000 / 180 + 500)

        S1_t = eval(ui1.lineEdit_66.text())
        S2_t = eval(ui1.lineEdit_67.text())
        S3_t = eval(ui1.lineEdit_68.text())
        S4_t = eval(ui1.lineEdit_69.text())
        S5_t = eval(ui1.lineEdit_70.text())
        strtemp="*#10241#*\r\nG4:S1:{} t{} ,S2:{} t{} ,S3:{} t{} ,S4:{} t{} ,S5:{} t{} ".format(S1_a,S1_t,S2_a,S2_t,
                                                                                                 S3_a,S3_t,S4_a,S4_t,
                                                                                                 S5_a,S5_t)
        serialCB.data_send(strtemp)
        time.sleep(0.1)


    if (ui1.lineEdit_71.text() != "") and (ui1.lineEdit_72.text() != "") and\
            (ui1.lineEdit_73.text() != "") and (ui1.lineEdit_74.text() != "")\
            and(ui1.lineEdit_75.text() != ""):
        S1_a = int(eval(ui1.lineEdit_71.text()) * 2000 / 180 + 500)
        S2_a = int(eval(ui1.lineEdit_72.text()) * 2000 / 180 + 500)
        S3_a = int(eval(ui1.lineEdit_73.text()) * 2000 / 180 + 500)
        S4_a = int(eval(ui1.lineEdit_74.text()) * 2000 / 180 + 500)
        S5_a = int(eval(ui1.lineEdit_75.text()) * 2000 / 180 + 500)

        S1_t = eval(ui1.lineEdit_76.text())
        S2_t = eval(ui1.lineEdit_77.text())
        S3_t = eval(ui1.lineEdit_78.text())
        S4_t = eval(ui1.lineEdit_79.text())
        S5_t = eval(ui1.lineEdit_80.text())
        strtemp="*#10241#*\r\nG5:S1:{} t{} ,S2:{} t{} ,S3:{} t{} ,S4:{} t{} ,S5:{} t{} ".format(S1_a,S1_t,S2_a,S2_t,
                                                                                                 S3_a,S3_t,S4_a,S4_t,
                                                                                                 S5_a,S5_t)
        serialCB.data_send(strtemp)
        time.sleep(0.1)

    if (ui1.lineEdit_81.text() != "") and (ui1.lineEdit_82.text() != "") and\
            (ui1.lineEdit_83.text() != "") and (ui1.lineEdit_84.text() != "")\
            and(ui1.lineEdit_85.text() != ""):
        S1_a = int(eval(ui1.lineEdit_81.text()) * 2000 / 180 + 500)
        S2_a = int(eval(ui1.lineEdit_82.text()) * 2000 / 180 + 500)
        S3_a = int(eval(ui1.lineEdit_83.text()) * 2000 / 180 + 500)
        S4_a = int(eval(ui1.lineEdit_84.text()) * 2000 / 180 + 500)
        S5_a = int(eval(ui1.lineEdit_85.text()) * 2000 / 180 + 500)

        S1_t = eval(ui1.lineEdit_86.text())
        S2_t = eval(ui1.lineEdit_87.text())
        S3_t = eval(ui1.lineEdit_88.text())
        S4_t = eval(ui1.lineEdit_89.text())
        S5_t = eval(ui1.lineEdit_90.text())
        strtemp="*#10241#*\r\nG6:S1:{} t{} ,S2:{} t{} ,S3:{} t{} ,S4:{} t{} ,S5:{} t{} ".format(S1_a,S1_t,S2_a,S2_t,
                                                                                                 S3_a,S3_t,S4_a,S4_t,
                                                                                                 S5_a,S5_t)
        serialCB.data_send(strtemp)
        time.sleep(0.1)

    if (ui1.lineEdit_91.text() != "") and (ui1.lineEdit_92.text() != "") and\
            (ui1.lineEdit_93.text() != "") and (ui1.lineEdit_94.text() != "")\
            and(ui1.lineEdit_95.text() != ""):
        S1_a = int(eval(ui1.lineEdit_91.text()) * 2000 / 180 + 500)
        S2_a = int(eval(ui1.lineEdit_92.text()) * 2000 / 180 + 500)
        S3_a = int(eval(ui1.lineEdit_93.text()) * 2000 / 180 + 500)
        S4_a = int(eval(ui1.lineEdit_94.text()) * 2000 / 180 + 500)
        S5_a = int(eval(ui1.lineEdit_95.text()) * 2000 / 180 + 500)

        S1_t = eval(ui1.lineEdit_96.text())
        S2_t = eval(ui1.lineEdit_97.text())
        S3_t = eval(ui1.lineEdit_98.text())
        S4_t = eval(ui1.lineEdit_99.text())
        S5_t = eval(ui1.lineEdit_100.text())
        strtemp="*#10241#*\r\nG7:S1:{} t{} ,S2:{} t{} ,S3:{} t{} ,S4:{} t{} ,S5:{} t{} ".format(S1_a,S1_t,S2_a,S2_t,
                                                                                                 S3_a,S3_t,S4_a,S4_t,
                                                                                                 S5_a,S5_t)
        serialCB.data_send(strtemp)
        time.sleep(0.1)

    if (ui1.lineEdit_101.text() != "") and (ui1.lineEdit_102.text() != "") and\
            (ui1.lineEdit_103.text() != "") and (ui1.lineEdit_104.text() != "")\
            and(ui1.lineEdit_105.text() != ""):
        S1_a = int(eval(ui1.lineEdit_101.text()) * 2000 / 180 + 500)
        S2_a = int(eval(ui1.lineEdit_102.text()) * 2000 / 180 + 500)
        S3_a = int(eval(ui1.lineEdit_103.text()) * 2000 / 180 + 500)
        S4_a = int(eval(ui1.lineEdit_104.text()) * 2000 / 180 + 500)
        S5_a = int(eval(ui1.lineEdit_105.text()) * 2000 / 180 + 500)

        S1_t = eval(ui1.lineEdit_106.text())
        S2_t = eval(ui1.lineEdit_107.text())
        S3_t = eval(ui1.lineEdit_108.text())
        S4_t = eval(ui1.lineEdit_109.text())
        S5_t = eval(ui1.lineEdit_110.text())
        strtemp="*#10241#*\r\nG8:S1:{} t{} ,S2:{} t{} ,S3:{} t{} ,S4:{} t{} ,S5:{} t{} ".format(S1_a,S1_t,S2_a,S2_t,
                                                                                                 S3_a,S3_t,S4_a,S4_t,
                                                                                                 S5_a,S5_t)
        serialCB.data_send(strtemp)
        time.sleep(0.1)

def saveGroupOrderToFlash(ui1 = uidesign.Ui_MainWindow()):
    command = 1
    if ui1.checkBox.isChecked():
        command = command | (1 << 2)
        if ui1.radioButton_4.isChecked():
            command = command | (1 << 3)

    sendingDic = {"Command": command}
    sendDataBuff = "{\"Command\":" + "{}".format(command)
    finishFlag = 0
    if (ui1.comboBox_3.currentText() != "") and (finishFlag != 1):
        sendDataBuff += ",\"" + ui1.comboBox_3.currentText() + "\":"
        sendDataBuff += "{}".format(
            eval(ui1.lineEdit_138.text()) if ui1.lineEdit_138.text() != "" else 255)
        sendingDic[ui1.comboBox_3.currentText()] = \
            eval(ui1.lineEdit_138.text()) if ui1.lineEdit_138.text() != "" else 255
        if ui1.lineEdit_138.text() == "":
            finishFlag = 1

    if (ui1.comboBox_4.currentText() != "") and (finishFlag != 1):
        sendDataBuff += ",\"" + ui1.comboBox_4.currentText() + "\":"
        sendDataBuff += "{}".format(
            eval(ui1.lineEdit_139.text()) if ui1.lineEdit_139.text() != "" else 255)
        sendingDic[ui1.comboBox_4.currentText()] = \
            eval(ui1.lineEdit_139.text()) if ui1.lineEdit_139.text() != "" else 255
        if ui1.lineEdit_139.text() == "":
            finishFlag = 1

    if (ui1.comboBox_5.currentText() != "") and (finishFlag != 1):
        sendDataBuff += ",\"" + ui1.comboBox_5.currentText() + "\":"
        sendDataBuff += "{}".format(
            eval(ui1.lineEdit_140.text()) if ui1.lineEdit_140.text() != "" else 255)
        sendingDic[ui1.comboBox_5.currentText()] = \
            eval(ui1.lineEdit_140.text()) if ui1.lineEdit_140.text() != "" else 255
        if ui1.lineEdit_140.text() == "":
            finishFlag = 1

    if (ui1.comboBox_6.currentText() != "") and (finishFlag != 1):
        sendDataBuff += ",\"" + ui1.comboBox_6.currentText() + "\":"
        sendDataBuff += "{}".format(
            eval(ui1.lineEdit_141.text()) if ui1.lineEdit_141.text() != "" else 255)
        sendingDic[ui1.comboBox_6.currentText()] = \
            eval(ui1.lineEdit_141.text()) if ui1.lineEdit_141.text() != "" else 255
        if ui1.lineEdit_141.text() == "":
            finishFlag = 1

    if (ui1.comboBox_7.currentText() != "") and (finishFlag != 1):
        sendDataBuff += ",\"" + ui1.comboBox_7.currentText() + "\":"
        sendDataBuff += "{}".format(
            eval(ui1.lineEdit_142.text()) if ui1.lineEdit_142.text() != "" else 255)
        sendingDic[ui1.comboBox_7.currentText()] = \
            eval(ui1.lineEdit_142.text()) if ui1.lineEdit_142.text() != "" else 255
        if ui1.lineEdit_142.text() == "":
            finishFlag = 1

    if (ui1.comboBox_8.currentText() != "") and (finishFlag != 1):
        sendDataBuff += ",\"" + ui1.comboBox_8.currentText() + "\":"
        sendDataBuff += "{}".format(
            eval(ui1.lineEdit_143.text()) if ui1.lineEdit_143.text() != "" else 255)
        sendingDic[ui1.comboBox_8.currentText()] = \
            eval(ui1.lineEdit_143.text()) if ui1.lineEdit_143.text() != "" else 255
        if ui1.lineEdit_143.text() == "":
            finishFlag = 1

    if (ui1.comboBox_9.currentText() != "") and (finishFlag != 1):
        sendDataBuff += ",\"" + ui1.comboBox_9.currentText() + "\":"
        sendDataBuff += "{}".format(
            eval(ui1.lineEdit_144.text()) if ui1.lineEdit_144.text() != "" else 255)
        sendingDic[ui1.comboBox_9.currentText()] = \
            eval(ui1.lineEdit_144.text()) if ui1.lineEdit_144.text() != "" else 255
        if ui1.lineEdit_144.text() == "":
            finishFlag = 1

    if (ui1.comboBox_10.currentText() != "") and (finishFlag != 1):
        sendDataBuff += ",\"" + ui1.comboBox_10.currentText() + "\":"
        sendDataBuff += "{}".format(
            eval(ui1.lineEdit_145.text()) if ui1.lineEdit_145.text() != "" else 255)
        sendingDic[ui1.comboBox_10.currentText()] = \
            eval(ui1.lineEdit_145.text()) if ui1.lineEdit_145.text() != "" else 255
        if ui1.lineEdit_145.text() == "":
            finishFlag = 1

    if (ui1.comboBox_11.currentText() != "") and (finishFlag != 1):
        sendDataBuff += ",\"" + ui1.comboBox_11.currentText() + "\":"
        sendDataBuff += "{}".format(
            eval(ui1.lineEdit_146.text()) if ui1.lineEdit_146.text() != "" else 255)
        sendingDic[ui1.comboBox_11.currentText()] = \
            eval(ui1.lineEdit_146.text()) if ui1.lineEdit_146.text() != "" else 255
        if ui1.lineEdit_146.text() == "":
            finishFlag = 1

    if (ui1.comboBox_12.currentText() != "") and (finishFlag != 1):
        sendDataBuff += ",\"" + ui1.comboBox_12.currentText() + "\":"
        sendDataBuff += "{}".format(
            eval(ui1.lineEdit_147.text()) if ui1.lineEdit_147.text() != "" else 255)
        sendingDic[ui1.comboBox_12.currentText()] = 255

    sendDataBuff += "}"
    serialCB.data_send("*#10242#*\r\n"+sendDataBuff)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    MainWindow = QMainWindow()
    ui = uidesign.Ui_MainWindow()
    ui.setupUi(MainWindow)
    ui_buttomInit(ui)
    serialCB = serialControl.Pyqt5_Serial(ui,MainWindow)
    refreshMission = RefreshInternet(ui)
    MainWindow.setFixedSize(MainWindow.width(), MainWindow.height())


    MainWindow.show()

    # 加载Wifi记忆文件
    try:
        with open('WiFiData.pydt', 'rb') as WiFids:
            changeWiFi.WiFiDataRem = pickle.load(WiFids)
            WiFids.close()
    except:
        print("WiFiData File Not Detected !")


    changeWiFiDialog = QDialog()
    changeWiFiDialog_ui = child_dialog.Ui_Child_WiFiSetting()
    changeWiFiDialog_ui.setupUi(changeWiFiDialog)
    WiFiConfig=changeWiFi.ChangeWiFi(changeWiFiDialog_ui,changeWiFiDialog,serialCB)

    childInputDialog = QDialog()
    childInputDialog_ui = InputGroupNum.Ui_Dialog()
    childInputDialog_ui.setupUi(Dialog=childInputDialog)
    savegroup=saveToGroup.save1(childInputDialog_ui,ui,childInputDialog)
    # WiFi设置按钮
    ui.pushButton_5.clicked.connect(changeWiFiDialog.show)
    #保存按钮
    ui.pushButton_2.clicked.connect(childInputDialog.show)

    # ui.lineEdit_27.textChanged()





    #保存到Flash按钮
    ui.pushButton_6.clicked.connect(lambda :saveGroupToFlash(ui))
    #保存顺序到Flash按钮
    ui.pushButton_7.clicked.connect(lambda :saveGroupOrderToFlash(ui))

    DH_Calcing=DH_Calc.calculatingDH(ui)

    #角度文本框改变

    # ui.lineEdit.textChanged.connect(DH_Calcing.draw2D)
    # ui.lineEdit_4.textChanged.connect(DH_Calcing.draw2D)
    # ui.lineEdit_12.textChanged.connect(DH_Calcing.draw2D)
    # ui.lineEdit_9.textChanged.connect(DH_Calcing.draw2D)
    # ui.lineEdit_16.textChanged.connect(DH_Calcing.draw2D)

    refreshMission.start()

    sys.exit(app.exec_())


