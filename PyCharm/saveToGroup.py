#saveToGroup.py

actGrouptoSave=0

class save1():
    def __init__(self,ui1,ui2,dialog):
        self.ui1 = ui1
        self.ui2 = ui2
        self.dialog = dialog

        self.init()

    def init(self):
        self.ui1.pushButton_3.clicked.connect(self.accept)
        self.ui1.pushButton_4.clicked.connect(self.refuse)

    def accept(self):
        index= self.ui1.comboBox_3.currentText()
        if index == "G1":
            self.ui2.lineEdit_20.setText(self.ui2.lineEdit.text())
            self.ui2.lineEdit_21.setText(self.ui2.lineEdit_4.text())
            self.ui2.lineEdit_22.setText(self.ui2.lineEdit_12.text())
            self.ui2.lineEdit_23.setText(self.ui2.lineEdit_9.text())
            self.ui2.lineEdit_24.setText(self.ui2.lineEdit_16.text())

            self.ui2.lineEdit_36.setText(self.ui2.lineEdit_2.text())
            self.ui2.lineEdit_37.setText(self.ui2.lineEdit_3.text())
            self.ui2.lineEdit_38.setText(self.ui2.lineEdit_10.text())
            self.ui2.lineEdit_39.setText(self.ui2.lineEdit_11.text())
            self.ui2.lineEdit_40.setText(self.ui2.lineEdit_14.text())
            self.ui2.tabWidget.setCurrentIndex(1)
            self.ui2.lineEdit_40.setFocus()

        elif index == "G2":
            self.ui2.lineEdit_41.setText(self.ui2.lineEdit.text())
            self.ui2.lineEdit_42.setText(self.ui2.lineEdit_4.text())
            self.ui2.lineEdit_43.setText(self.ui2.lineEdit_12.text())
            self.ui2.lineEdit_44.setText(self.ui2.lineEdit_9.text())
            self.ui2.lineEdit_45.setText(self.ui2.lineEdit_16.text())

            self.ui2.lineEdit_46.setText(self.ui2.lineEdit_2.text())
            self.ui2.lineEdit_47.setText(self.ui2.lineEdit_3.text())
            self.ui2.lineEdit_48.setText(self.ui2.lineEdit_10.text())
            self.ui2.lineEdit_49.setText(self.ui2.lineEdit_11.text())
            self.ui2.lineEdit_50.setText(self.ui2.lineEdit_14.text())
            self.ui2.tabWidget.setCurrentIndex(1)
            self.ui2.lineEdit_50.setFocus()

        elif index == "G3":
            self.ui2.lineEdit_51.setText(self.ui2.lineEdit.text())
            self.ui2.lineEdit_52.setText(self.ui2.lineEdit_4.text())
            self.ui2.lineEdit_53.setText(self.ui2.lineEdit_12.text())
            self.ui2.lineEdit_54.setText(self.ui2.lineEdit_9.text())
            self.ui2.lineEdit_55.setText(self.ui2.lineEdit_16.text())

            self.ui2.lineEdit_56.setText(self.ui2.lineEdit_2.text())
            self.ui2.lineEdit_57.setText(self.ui2.lineEdit_3.text())
            self.ui2.lineEdit_58.setText(self.ui2.lineEdit_10.text())
            self.ui2.lineEdit_59.setText(self.ui2.lineEdit_11.text())
            self.ui2.lineEdit_60.setText(self.ui2.lineEdit_14.text())
            self.ui2.tabWidget.setCurrentIndex(1)
            self.ui2.lineEdit_60.setFocus()

        elif index == "G4":
            self.ui2.lineEdit_61.setText(self.ui2.lineEdit.text())
            self.ui2.lineEdit_62.setText(self.ui2.lineEdit_4.text())
            self.ui2.lineEdit_63.setText(self.ui2.lineEdit_12.text())
            self.ui2.lineEdit_64.setText(self.ui2.lineEdit_9.text())
            self.ui2.lineEdit_65.setText(self.ui2.lineEdit_16.text())

            self.ui2.lineEdit_66.setText(self.ui2.lineEdit_2.text())
            self.ui2.lineEdit_67.setText(self.ui2.lineEdit_3.text())
            self.ui2.lineEdit_68.setText(self.ui2.lineEdit_10.text())
            self.ui2.lineEdit_69.setText(self.ui2.lineEdit_11.text())
            self.ui2.lineEdit_70.setText(self.ui2.lineEdit_14.text())
            self.ui2.tabWidget.setCurrentIndex(1)
            self.ui2.lineEdit_70.setFocus()

        elif index == "G5":
            self.ui2.lineEdit_71.setText(self.ui2.lineEdit.text())
            self.ui2.lineEdit_72.setText(self.ui2.lineEdit_4.text())
            self.ui2.lineEdit_73.setText(self.ui2.lineEdit_12.text())
            self.ui2.lineEdit_74.setText(self.ui2.lineEdit_9.text())
            self.ui2.lineEdit_75.setText(self.ui2.lineEdit_16.text())

            self.ui2.lineEdit_76.setText(self.ui2.lineEdit_2.text())
            self.ui2.lineEdit_77.setText(self.ui2.lineEdit_3.text())
            self.ui2.lineEdit_78.setText(self.ui2.lineEdit_10.text())
            self.ui2.lineEdit_79.setText(self.ui2.lineEdit_11.text())
            self.ui2.lineEdit_80.setText(self.ui2.lineEdit_14.text())
            self.ui2.tabWidget.setCurrentIndex(1)
            self.ui2.lineEdit_80.setFocus()

        elif index == "G6":
            self.ui2.lineEdit_81.setText(self.ui2.lineEdit.text())
            self.ui2.lineEdit_82.setText(self.ui2.lineEdit_4.text())
            self.ui2.lineEdit_83.setText(self.ui2.lineEdit_12.text())
            self.ui2.lineEdit_84.setText(self.ui2.lineEdit_9.text())
            self.ui2.lineEdit_85.setText(self.ui2.lineEdit_16.text())

            self.ui2.lineEdit_86.setText(self.ui2.lineEdit_2.text())
            self.ui2.lineEdit_87.setText(self.ui2.lineEdit_3.text())
            self.ui2.lineEdit_88.setText(self.ui2.lineEdit_10.text())
            self.ui2.lineEdit_89.setText(self.ui2.lineEdit_11.text())
            self.ui2.lineEdit_90.setText(self.ui2.lineEdit_14.text())
            self.ui2.tabWidget.setCurrentIndex(1)
            self.ui2.lineEdit_90.setFocus()

        elif index == "G7":
            self.ui2.lineEdit_91.setText(self.ui2.lineEdit.text())
            self.ui2.lineEdit_92.setText(self.ui2.lineEdit_4.text())
            self.ui2.lineEdit_93.setText(self.ui2.lineEdit_12.text())
            self.ui2.lineEdit_94.setText(self.ui2.lineEdit_9.text())
            self.ui2.lineEdit_95.setText(self.ui2.lineEdit_16.text())

            self.ui2.lineEdit_96.setText(self.ui2.lineEdit_2.text())
            self.ui2.lineEdit_97.setText(self.ui2.lineEdit_3.text())
            self.ui2.lineEdit_98.setText(self.ui2.lineEdit_10.text())
            self.ui2.lineEdit_99.setText(self.ui2.lineEdit_11.text())
            self.ui2.lineEdit_100.setText(self.ui2.lineEdit_14.text())
            self.ui2.tabWidget.setCurrentIndex(1)
            self.ui2.lineEdit_100.setFocus()

        elif index == "G8":
            self.ui2.lineEdit_101.setText(self.ui2.lineEdit.text())
            self.ui2.lineEdit_102.setText(self.ui2.lineEdit_4.text())
            self.ui2.lineEdit_103.setText(self.ui2.lineEdit_12.text())
            self.ui2.lineEdit_104.setText(self.ui2.lineEdit_9.text())
            self.ui2.lineEdit_105.setText(self.ui2.lineEdit_16.text())

            self.ui2.lineEdit_106.setText(self.ui2.lineEdit_2.text())
            self.ui2.lineEdit_107.setText(self.ui2.lineEdit_3.text())
            self.ui2.lineEdit_108.setText(self.ui2.lineEdit_10.text())
            self.ui2.lineEdit_109.setText(self.ui2.lineEdit_11.text())
            self.ui2.lineEdit_110.setText(self.ui2.lineEdit_14.text())
            self.ui2.tabWidget.setCurrentIndex(1)
            self.ui2.lineEdit_110.setFocus()

        self.dialog.close()

    def refuse(self):
        self.dialog.close()


