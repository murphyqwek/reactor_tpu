import sys
from PyQt5 import QtWidgets
from PyQt5.QtCore import QThread
import connect
import start
import param
from port import serial_ports, speeds
import serial
import threading


class ConnectApp(QtWidgets.QMainWindow, connect.Ui_Form):
    realport = None

    def __init__(self):
        super().__init__()
        self.ParamApp = None
        self.setupUi(self)
        self.Port.addItems(serial_ports())
        self.Speed.addItems(speeds)
        self.StartApp = None
        self.ConnectButton.clicked.connect(self.connecting)
        self.RefreshButton.clicked.connect(self.refresh)

    def connecting(self):
        try:
            self.ParamApp = ParamApp()
            ConnectApp.realport = serial.Serial(self.Port.currentText(), int(self.Speed.currentText()))
            self.ConnectButton.setText('Подключено')
            self.ParamApp.show()
            self.hide()
        except Exception as e:
            print(e)

    def refresh(self):
        self.Port.clear()
        self.Port.addItems(serial_ports())


class ParamApp(QtWidgets.QWidget, param.Ui_Form):
    setTime = ""
    setCur = ""
    setMode = ""

    def __init__(self):
        super().__init__()
        self.ConnectApp = None
        self.setupUi(self)
        self.realport = None
        self.DisconnectButton.clicked.connect(self.disconnect)
        self.StartButton.clicked.connect(self.starting)
        self.StartApp = None
        self.time_sint.valueChanged[int].connect(self.changeValue)

    def starting(self):
        self.StartApp = StartApp()
        self.StartApp.label_timer.setText(self.label_timer.text())
        ParamApp.setTime = str(self.label_timer.text()[:-2])
        self.StartApp.label_curent.setText(self.cur.currentText())
        ParamApp.setCur = str(self.cur.currentIndex())
        self.StartApp.label_workmod.setText(self.mode.currentText())
        ParamApp.setMode = str(self.mode.currentIndex())
        self.StartApp.show()
        self.hide()

    def changeValue(self, value):
        self.label_timer.setText(str(value) + " с")

    def disconnect(self):
        try:
            self.ConnectApp = ConnectApp()
            serial.Serial.close(ConnectApp.realport)
            self.ConnectApp.ConnectButton.setText('Подключить')
            self.ConnectApp.show()
            self.hide()
        except Exception as e:
            print(e)


class StartApp(QtWidgets.QWidget, start.Ui_Form):

    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.DisconnectButton.clicked.connect(self.back)
        self.StartButton.setCheckable(True)
        self.StartButton.clicked.connect(self.start)
        self.StartButton.clicked.connect(self.stop)
        self.realport = ConnectApp.realport
        self.file = None
        self.file_text = ""
        self.ReadPort()
        self.ParamApp = None
        self.brk = False

    def back(self):
        threading.Timer(1, self.ReadPort).cancel()
        self.ParamApp = ParamApp()
        self.ParamApp.show()
        self.hide()

    def start(self):
        if self.StartButton.isChecked():
            self.label_statusnow.setText("Передаю параметры")
            self.StartButton.setStyleSheet("background-color: red")
            self.StartButton.setText('Стоп')
            self.DisconnectButton.hide()
            self.realport.write(b'1')
            self.realport.write(bytes(ParamApp.setTime, encoding='utf-8') + b'n')
            self.realport.write(bytes(ParamApp.setCur, encoding='utf-8'))
            self.realport.write(bytes(ParamApp.setMode, encoding='utf-8'))
            self.realport.write(b'e')
            self.realport.write(b's')
            self.label_statusnow.setText("В работе")

    def stop(self):
        if not self.StartButton.isChecked():
            self.label_statusnow.setText("Ожидание")
            self.StartButton.setStyleSheet("background-color: green")
            self.DisconnectButton.show()
            self.StartButton.setText('Старт')
            self.realport.write(b'd')
            self.file = open("otus.txt", "w+")
            while self.realport.in_waiting:
                self.file_text += self.realport.read().decode()
                self.file.write(str(self.file_text))
            if self.brk:
                self.brk = False
                self.file.write(str(self.file_text))
            self.file.close()
            self.file_text = ""

    def ReadPort(self):
        threading.Timer(1, self.ReadPort).start()
        while self.realport.in_waiting:
            self.file_text += self.realport.read().decode()
        if "end" in self.file_text:
            self.brk = True
            self.StartButton.setChecked(False)
            self.stop()

def main():
    app = QtWidgets.QApplication(sys.argv)
    window = ConnectApp()
    window.show()
    app.exec_()


if __name__ == '__main__':
    main()
