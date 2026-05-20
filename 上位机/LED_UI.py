# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'LED_UI.ui'
##
## Created by: Qt User Interface Compiler version 6.11.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QComboBox, QLabel, QMainWindow,
    QMenuBar, QPushButton, QSizePolicy, QSpinBox,
    QStatusBar, QVBoxLayout, QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.LED_ON = QPushButton(self.centralwidget)
        self.LED_ON.setObjectName(u"LED_ON")
        self.LED_ON.setGeometry(QRect(100, 340, 131, 51))
        font = QFont()
        font.setPointSize(20)
        font.setBold(True)
        self.LED_ON.setFont(font)
        self.LED_ON.setIconSize(QSize(18, 18))
        self.LED_OFF = QPushButton(self.centralwidget)
        self.LED_OFF.setObjectName(u"LED_OFF")
        self.LED_OFF.setGeometry(QRect(520, 330, 131, 51))
        self.LED_OFF.setFont(font)
        self.LED_OFF.setIconSize(QSize(18, 18))
        self.LED_OFF.setAutoRepeatDelay(300)
        self.label = QLabel(self.centralwidget)
        self.label.setObjectName(u"label")
        self.label.setGeometry(QRect(270, 180, 221, 71))
        font1 = QFont()
        font1.setPointSize(20)
        self.label.setFont(font1)
        self.label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.verticalLayoutWidget = QWidget(self.centralwidget)
        self.verticalLayoutWidget.setObjectName(u"verticalLayoutWidget")
        self.verticalLayoutWidget.setGeometry(QRect(10, 10, 201, 151))
        self.verticalLayout = QVBoxLayout(self.verticalLayoutWidget)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.refresh_button = QPushButton(self.verticalLayoutWidget)
        self.refresh_button.setObjectName(u"refresh_button")
        font2 = QFont()
        font2.setPointSize(12)
        font2.setBold(False)
        self.refresh_button.setFont(font2)

        self.verticalLayout.addWidget(self.refresh_button)

        self.cbox_port = QComboBox(self.verticalLayoutWidget)
        self.cbox_port.setObjectName(u"cbox_port")
        font3 = QFont()
        font3.setPointSize(15)
        self.cbox_port.setFont(font3)

        self.verticalLayout.addWidget(self.cbox_port)

        self.open_port = QPushButton(self.verticalLayoutWidget)
        self.open_port.setObjectName(u"open_port")
        font4 = QFont()
        font4.setPointSize(12)
        font4.setBold(False)
        font4.setItalic(False)
        self.open_port.setFont(font4)

        self.verticalLayout.addWidget(self.open_port)

        self.verticalLayoutWidget_2 = QWidget(self.centralwidget)
        self.verticalLayoutWidget_2.setObjectName(u"verticalLayoutWidget_2")
        self.verticalLayoutWidget_2.setGeometry(QRect(300, 360, 160, 89))
        self.verticalLayout_2 = QVBoxLayout(self.verticalLayoutWidget_2)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.verticalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.Count = QSpinBox(self.verticalLayoutWidget_2)
        self.Count.setObjectName(u"Count")
        font5 = QFont()
        font5.setPointSize(18)
        self.Count.setFont(font5)

        self.verticalLayout_2.addWidget(self.Count)

        self.Send = QPushButton(self.verticalLayoutWidget_2)
        self.Send.setObjectName(u"Send")
        font6 = QFont()
        font6.setPointSize(12)
        self.Send.setFont(font6)

        self.verticalLayout_2.addWidget(self.Send)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 800, 33))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.LED_ON.setText(QCoreApplication.translate("MainWindow", u"ON", None))
        self.LED_OFF.setText(QCoreApplication.translate("MainWindow", u"OFF", None))
        self.label.setText(QCoreApplication.translate("MainWindow", u"\u63a7\u4e2a\u706f\u5427", None))
        self.refresh_button.setText(QCoreApplication.translate("MainWindow", u"\u5237\u65b0", None))
        self.open_port.setText(QCoreApplication.translate("MainWindow", u"\u6253\u5f00\u4e32\u53e3", None))
        self.Send.setText(QCoreApplication.translate("MainWindow", u"\u53d1\u9001", None))
    # retranslateUi

