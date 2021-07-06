import child_dialog,pickle
from PyQt5 import QtWidgets

global WiFiDataRem

SSID = "706"
PWD = "741852963.0"
IotAddress = "dtu.heclouds.com"
IotPort = "1811"

class ChangeWiFi():
    def __init__(self, ui,dialog,serial):
        self.ui = ui
        self.dialog=dialog
        self.serial=serial
        self.init()

    def init(self):
        #恢复默认按键绑定
        self.ui.checkBox.setChecked(WiFiDataRem[0])
        self.ui.lineEdit.setText(WiFiDataRem[1])
        self.ui.lineEdit_3.setText(WiFiDataRem[2])
        self.ui.lineEdit_4.setText(WiFiDataRem[3])
        self.ui.pushButton_18.clicked.connect(self.Reset)
        self.ui.pushButton_17.clicked.connect(self.saveAndSend)

    def Reset(self):
        self.ui.checkBox.setChecked(True)
        self.ui.lineEdit.setText(IotAddress)
        self.ui.lineEdit_2.setText(IotPort)
        self.ui.lineEdit_3.setText(SSID)
        self.ui.lineEdit_4.setText(PWD)

    def saveAndSend(self):
        enableState = 1 if self.ui.checkBox.isChecked() else 0
        sendString = "*#10248#*\r\n"+"{"+"\"SSID\":\""+self.ui.lineEdit_3.text()\
                     +"\","+"\"PWD\":\""+self.ui.lineEdit_4.text()+"\""+",\"Enable\":"\
                     +str(enableState)+"}"
        WiFiDataRem = [self.ui.checkBox.isChecked(),self.ui.lineEdit.text(),self.ui.lineEdit_3.text(),\
                       self.ui.lineEdit_4.text()]
        try:
            with open('WiFiData.pydt', 'wb') as text:
                pickle.dump(WiFiDataRem, text)
                text.close()
                print("Save to Local Succeed ! ")
        except:
            print("Save to Local Fail ! ")
        print(sendString)
        self.serial.data_send(sendString)
        self.dialog.close()