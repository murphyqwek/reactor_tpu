from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(540, 200)
        self.gridLayoutWidget = QtWidgets.QWidget(Form)
        self.gridLayoutWidget.setGeometry(QtCore.QRect(20, 20, 500, 160))
        self.gridLayoutWidget.setObjectName("gridLayoutWidget")
        self.gridLayout_2 = QtWidgets.QGridLayout(self.gridLayoutWidget)
        self.gridLayout_2.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.label_statusnow = QtWidgets.QLabel(self.gridLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.label_statusnow.setFont(font)
        self.label_statusnow.setStyleSheet("color: rgb(0,195,0);")
        self.label_statusnow.setObjectName("label_statusnow")
        self.gridLayout_2.addWidget(self.label_statusnow, 2, 1, 1, 1)
        self.label_workmod = QtWidgets.QLabel(self.gridLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_workmod.setFont(font)
        self.label_workmod.setObjectName("label_workmod")
        self.gridLayout_2.addWidget(self.label_workmod, 1, 2, 1, 1)
        self.label_mode = QtWidgets.QLabel(self.gridLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(15)
        self.label_mode.setFont(font)
        self.label_mode.setObjectName("label_mode")
        self.gridLayout_2.addWidget(self.label_mode, 0, 2, 1, 1)
        self.label_status = QtWidgets.QLabel(self.gridLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(15)
        self.label_status.setFont(font)
        self.label_status.setObjectName("label_status")
        self.gridLayout_2.addWidget(self.label_status, 2, 0, 1, 1)
        self.label_cur = QtWidgets.QLabel(self.gridLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(15)
        self.label_cur.setFont(font)
        self.label_cur.setObjectName("label_cur")
        self.gridLayout_2.addWidget(self.label_cur, 0, 1, 1, 1)
        self.label_time = QtWidgets.QLabel(self.gridLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(15)
        self.label_time.setFont(font)
        self.label_time.setObjectName("label_time")
        self.gridLayout_2.addWidget(self.label_time, 0, 0, 1, 1)
        self.label_timer = QtWidgets.QLabel(self.gridLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_timer.setFont(font)
        self.label_timer.setObjectName("label_timer")
        self.gridLayout_2.addWidget(self.label_timer, 1, 0, 1, 1)
        self.label_curent = QtWidgets.QLabel(self.gridLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_curent.setFont(font)
        self.label_curent.setObjectName("label_curent")
        self.gridLayout_2.addWidget(self.label_curent, 1, 1, 1, 1)
        self.StartButton = QtWidgets.QPushButton(self.gridLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.StartButton.setFont(font)
        self.StartButton.setStyleSheet("background-color: rgb(0,195,0);")
        self.StartButton.setObjectName("StartButton")
        self.gridLayout_2.addWidget(self.StartButton, 2, 2, 1, 1)
        self.DisconnectButton = QtWidgets.QPushButton(self.gridLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.DisconnectButton.setFont(font)
        self.DisconnectButton.setStyleSheet("background-color: rgb(200,200,200);")
        self.DisconnectButton.setObjectName("DisconnectButton")
        self.gridLayout_2.addWidget(self.DisconnectButton, 3, 0, 1, 1)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "??????????"))
        self.label_statusnow.setText(_translate("Form", "????????????????"))
        self.label_workmod.setText(_translate("Form", "2 ??????????"))
        self.label_mode.setText(_translate("Form", "??????????"))
        self.label_status.setText(_translate("Form", "????????????:"))
        self.label_cur.setText(_translate("Form", "??????"))
        self.label_time.setText(_translate("Form", "??????????"))
        self.label_timer.setText(_translate("Form", "5 ??"))
        self.label_curent.setText(_translate("Form", "200 A"))
        self.StartButton.setText(_translate("Form", "??????????"))
        self.DisconnectButton.setText(_translate("Form", "??????????"))
