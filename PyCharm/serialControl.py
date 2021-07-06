import sys
import serial
import serial.tools.list_ports

#from main import ui
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QMessageBox
from PyQt5.QtCore import QTimer
import uidesign
import graduation

serialStatus = 0



class Pyqt5_Serial():
    def __init__(self, ui, mainWnd):
        self.ui = ui
        self.mainWnd = mainWnd

        self.grapChanged=0
        # self.grapAngle=1000-5*ui.lineEdit_27.text()
        self.grapAngle=1000-5*0

        self.init()
        self.ser = serial.Serial()
        self.port_check()

        # 接收数据和发送数据数目置零
        self.data_num_received = 0
        ui.label_38.setText(str(self.data_num_received))
        self.data_num_sended = 0
        ui.label_39.setText(str(self.data_num_sended))

    def init(self):
        # 串口检测按钮
        self.ui.pushButton_4.clicked.connect(self.port_check)

        # 打开串口按钮
        self.ui.pushButton_15.setEnabled(False)
        self.ui.pushButton_15.clicked.connect(self.port_open)

        # 关闭串口按钮
        self.ui.pushButton_16.setEnabled(False)
        self.ui.pushButton_16.clicked.connect(self.port_close)

        # 发送数据按钮
        self.ui.pushButton.clicked.connect(self.pushSend)

        self.ui.pushButton_6.setEnabled(False)
        self.ui.pushButton_7.setEnabled(False)
        # # 定时发送数据
        # self.timer_send = QTimer()
        # self.timer_send.timeout.connect(self.data_send)
        # self.timer_send_cb.stateChanged.connect(self.data_send_timer)

        # 定时器接收数据
        self.timer = QTimer()
        self.timer.timeout.connect(self.data_receive)

        # # 清除发送窗口
        # self.s3__clear_button.clicked.connect(self.send_data_clear)

        # 清除接收窗口
        self.ui.pushButton_13.clicked.connect(self.receive_data_clear)

        self.ui.lineEdit_27.setText("0")
        self.ui.lineEdit_28.setText("1000")
        self.ui.lineEdit_27.textChanged.connect(self.grapChangeEvent)

    def grapChangeEvent(self):
        self.grapChanged = 1


    # 串口检测
    def port_check(self):
        # 检测所有存在的串口，将信息存储在字典中
        self.Com_Dict = {}
        port_list = list(serial.tools.list_ports.comports())
        self.ui.comboBox.clear()
        if len(port_list)!=0:
            self.ui.pushButton_15.setEnabled(True)
            for port in port_list:
                self.Com_Dict["%s" % port[0]] = "%s" % port[1]
                self.ui.comboBox.addItem(port[0])
        if len(self.Com_Dict) == 0:
            self.ui.label_41.setText(" 无串口")
            self.ui.pushButton_15.setEnabled(False)
            self.ui.pushButton_16.setEnabled(False)

    # # 串口信息
    # def port_imf(self):
    #     # 显示选定的串口的详细信息
    #     imf_s = self.s1__box_2.currentText()
    #     if imf_s != "":
    #         self.state_label.setText(self.Com_Dict[self.s1__box_2.currentText()])

    # 打开串口
    def port_open(self):
        self.ser.port = self.ui.comboBox.currentText()
        self.ser.baudrate = int(self.ui.comboBox_2.currentText())
        self.ser.bytesize = 8
        self.ser.stopbits = 1
        self.ser.parity = "N"

        try:
            self.ser.open()
            self.ser.setDTR(True)
            self.ser.setRTS(False)
        except:
            #QMessageBox.critical(QtWidgets, "Port Error", "此串口不能被打开！")
            return None

        # 打开串口接收定时器，周期为2ms
        self.timer.start(2)

        if self.ser.isOpen():
            self.ui.pushButton_15.setEnabled(False)
            self.ui.pushButton_16.setEnabled(True)
            self.ui.pushButton_5.setEnabled(True)
            self.ui.comboBox.setEnabled(False)
            self.ui.comboBox_2.setEnabled(False)
            serialStatus = 1
            self.ui.pushButton_6.setEnabled(True)
            self.ui.pushButton_7.setEnabled(True)
            self.ui.label_11.setText("串口日志（串口已开启）")

    # 关闭串口
    def port_close(self):
        self.timer.stop()
        # self.timer_send.stop()
        try:
            self.ser.close()
        except:
            pass
        self.ui.pushButton_15.setEnabled(True)
        self.ui.pushButton_16.setEnabled(False)
        self.ui.pushButton_5.setEnabled(False)
        self.ui.comboBox.setEnabled(True)
        self.ui.comboBox_2.setEnabled(True)
        # self.lineEdit_3.setEnabled(True)
        serialStatus = 0
        self.ui.pushButton_6.setEnabled(False)
        self.ui.pushButton_7.setEnabled(False)
        self.ui.label_11.setText("串口日志（串口已关闭）")

       # 发送数据
    def data_send(self,msgToSend):
        if self.ser.isOpen():
            input_s = msgToSend
            if input_s != "":
                # 非空字符串
                # ascii发送
                input_s = (input_s).encode('utf-8','ignore')

                num = self.ser.write(input_s)
                self.data_num_sended += num
                self.ui.label_39.setText(str(self.data_num_sended))
        else:
            pass

    def  pushSend(self):
        if self.ui.tabWidget.currentIndex() == 0 :
            S1_a = int(eval(self.ui.lineEdit.text())*2000/180+500)
            S2_a = int(eval(self.ui.lineEdit_4.text()) * 2000 / 180 + 500)
            S3_a = int(eval(self.ui.lineEdit_12.text()) * 2000 / 180 + 500)
            S4_a = int(eval(self.ui.lineEdit_9.text()) * 2000 / 180 + 500)
            S5_a = int(eval(self.ui.lineEdit_16.text()) * 2000 / 180 + 500)

            S1_t = eval(self.ui.lineEdit_2.text())
            S2_t = eval(self.ui.lineEdit_3.text())
            S3_t = eval(self.ui.lineEdit_10.text())
            S4_t = eval(self.ui.lineEdit_11.text())
            S5_t = eval(self.ui.lineEdit_14.text())

            actGroupDic={"S1":{"a":S1_a,"t":S1_t},"S2":{"a":S2_a,"t":S2_t},
                        "S3":{"a":S3_a,"t":S3_t},"S4":{"a":S4_a,"t":S4_t},
                        "S5":{"a":S5_a,"t":S5_t}}

            if self.grapChanged >= 1:
                self.grapChanged=0
                S6_t = eval(self.ui.lineEdit_28.text())
                self.grapAngle = 0 if self.ui.lineEdit_27.text() == "" else (
                            1000 - 5 * eval(self.ui.lineEdit_27.text()))
                actGroupDic["S6"] = {"a":self.grapAngle,"t":S6_t}

            sendingBuff=graduation.packActGroup(actGroupDic)


            print(sendingBuff)

            if self.ui.radioButton.isChecked():
                self.data_send(sendingBuff)
            elif self.ui.radioButton_2.isChecked():
                graduation.sendActGroupOnnet(actGroupDic)
            else:
                return
        if self.ui.tabWidget.currentIndex() == 1:
            command=0
            if (self.ui.lineEdit_138.text()!=""):
                command = 1<<1
            if self.ui.checkBox.isChecked():
                command = command|(1<<2)
                if self.ui.radioButton_4.isChecked():
                    command = command|(1<<3)

            sendingDic={"Command":"{}".format(command)}
            sendDataBuff="{\"Command\":"+"{}".format(command)
            finishFlag=0
            if (self.ui.comboBox_3.currentText()!="")and(finishFlag!=1):
                sendDataBuff+=",\""+self.ui.comboBox_3.currentText()+"\":"
                sendDataBuff+="{}".format(
                    eval(self.ui.lineEdit_138.text()) if self.ui.lineEdit_138.text()!="" else 255)
                sendingDic[self.ui.comboBox_3.currentText()] = \
                    eval(self.ui.lineEdit_138.text()) if self.ui.lineEdit_138.text()!="" else 255
                if self.ui.lineEdit_138.text() == "":
                    finishFlag=1

            if (self.ui.comboBox_4.currentText()!="")and(finishFlag!=1):
                sendDataBuff += ",\""+self.ui.comboBox_4.currentText() + "\":"
                sendDataBuff += "{}".format(
                    eval(self.ui.lineEdit_139.text()) if self.ui.lineEdit_139.text() != "" else 255)
                sendingDic[self.ui.comboBox_4.currentText()] = \
                    eval(self.ui.lineEdit_139.text()) if self.ui.lineEdit_139.text()!="" else 255
                if self.ui.lineEdit_139.text() == "":
                    finishFlag=1

            if (self.ui.comboBox_5.currentText()!="")and(finishFlag!=1):
                sendDataBuff += ",\""+self.ui.comboBox_5.currentText() + "\":"
                sendDataBuff += "{}".format(
                    eval(self.ui.lineEdit_140.text()) if self.ui.lineEdit_140.text() != "" else 255)
                sendingDic[self.ui.comboBox_5.currentText()] = \
                    eval(self.ui.lineEdit_140.text()) if self.ui.lineEdit_140.text()!="" else 255
                if self.ui.lineEdit_140.text() == "":
                    finishFlag=1

            if (self.ui.comboBox_6.currentText()!="")and(finishFlag!=1):
                sendDataBuff += ",\""+self.ui.comboBox_6.currentText() + "\":"
                sendDataBuff += "{}".format(
                    eval(self.ui.lineEdit_141.text()) if self.ui.lineEdit_141.text() != "" else 255)
                sendingDic[self.ui.comboBox_6.currentText()] = \
                    eval(self.ui.lineEdit_141.text()) if self.ui.lineEdit_141.text()!="" else 255
                if self.ui.lineEdit_141.text() == "":
                    finishFlag=1

            if (self.ui.comboBox_7.currentText()!="")and(finishFlag!=1):
                sendDataBuff += ",\""+self.ui.comboBox_7.currentText() + "\":"
                sendDataBuff += "{}".format(
                    eval(self.ui.lineEdit_142.text()) if self.ui.lineEdit_142.text() != "" else 255)
                sendingDic[self.ui.comboBox_7.currentText()] = \
                    eval(self.ui.lineEdit_142.text()) if self.ui.lineEdit_142.text()!="" else 255
                if self.ui.lineEdit_142.text() == "":
                    finishFlag=1

            if (self.ui.comboBox_8.currentText()!="")and(finishFlag!=1):
                sendDataBuff += ",\""+self.ui.comboBox_8.currentText() + "\":"
                sendDataBuff += "{}".format(
                    eval(self.ui.lineEdit_143.text()) if self.ui.lineEdit_143.text() != "" else 255)
                sendingDic[self.ui.comboBox_8.currentText()] = \
                    eval(self.ui.lineEdit_143.text()) if self.ui.lineEdit_143.text()!="" else 255
                if self.ui.lineEdit_143.text() == "":
                    finishFlag=1

            if (self.ui.comboBox_9.currentText()!="")and(finishFlag!=1):
                sendDataBuff += ",\""+self.ui.comboBox_9.currentText() + "\":"
                sendDataBuff += "{}".format(
                    eval(self.ui.lineEdit_144.text()) if self.ui.lineEdit_144.text() != "" else 255)
                sendingDic[self.ui.comboBox_9.currentText()] = \
                    eval(self.ui.lineEdit_144.text()) if self.ui.lineEdit_144.text()!="" else 255
                if self.ui.lineEdit_144.text() == "":
                    finishFlag=1

            if (self.ui.comboBox_10.currentText()!="")and(finishFlag!=1):
                sendDataBuff += ",\""+self.ui.comboBox_10.currentText() + "\":"
                sendDataBuff += "{}".format(
                    eval(self.ui.lineEdit_145.text()) if self.ui.lineEdit_145.text() != "" else 255)
                sendingDic[self.ui.comboBox_10.currentText()] = \
                    eval(self.ui.lineEdit_145.text()) if self.ui.lineEdit_145.text()!="" else 255
                if self.ui.lineEdit_145.text() == "":
                    finishFlag=1

            if (self.ui.comboBox_11.currentText()!="")and(finishFlag!=1):
                sendDataBuff += ",\""+self.ui.comboBox_11.currentText() + "\":"
                sendDataBuff += "{}".format(
                    eval(self.ui.lineEdit_146.text()) if self.ui.lineEdit_146.text() != "" else 255)
                sendingDic[self.ui.comboBox_11.currentText()] = \
                    eval(self.ui.lineEdit_146.text()) if self.ui.lineEdit_146.text()!="" else 255
                if self.ui.lineEdit_146.text() == "":
                    finishFlag=1

            if (self.ui.comboBox_12.currentText()!="")and(finishFlag!=1):
                sendDataBuff += ",\""+self.ui.comboBox_12.currentText() + "\":"
                sendDataBuff += "{}".format(
                    eval(self.ui.lineEdit_147.text()) if self.ui.lineEdit_147.text() != "" else 255)
                sendingDic[self.ui.comboBox_12.currentText()] = 255
            
            sendDataBuff+="}"
            #print(graduation.packOtherGroup("*#10242#*\r\n",sendingDic))
            if self.ui.radioButton.isChecked():
                # self.data_send(graduation.packOtherGroup("*#10242#*\r\n",sendingDic))
                self.data_send("*#10242#*\r\n"+sendDataBuff)
            elif self.ui.radioButton_2.isChecked():
                graduation.sendOtherCommandOnnet("*#10242#*\r\n",sendingDic)
            else:
                return

    # 接收数据
    def data_receive(self):
        try:
            num = self.ser.inWaiting()
        except:
            self.port_close()
            return None
        if num > 0:
            data = self.ser.read(num)
            num = len(data)
            self.ui.textBrowser.insertPlainText(data.decode('utf-8','ignore'))

            # 统计接收字符的数量
            self.data_num_received += num
            self.ui.label_38.setText(str(self.data_num_received))

            # 获取到text光标
            textCursor = self.ui.textBrowser.textCursor()
            # 滚动到底部
            textCursor.movePosition(textCursor.End)
            # 设置光标到text中去
            self.ui.textBrowser.setTextCursor(textCursor)
        else:
            pass

    # # 定时发送数据
    # def data_send_timer(self):
    #     if self.timer_send_cb.isChecked():
    #         self.timer_send.start(int(self.lineEdit_3.text()))
    #         self.lineEdit_3.setEnabled(False)
    #     else:
    #         self.timer_send.stop()
    #         self.lineEdit_3.setEnabled(True)

    # 清除显示
    def send_data_clear(self):
        pass
        # self.s3__send_text.setText("")

    def receive_data_clear(self):
        self.ui.textBrowser.setText("")
        self.data_num_received=0
        self.data_num_sended=0
        self.ui.label_38.setText(str(self.data_num_received))
        self.ui.label_39.setText(str(self.data_num_sended))


