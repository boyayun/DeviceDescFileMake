#-------------------------------------------------
#
# Project created by QtCreator 2016-04-21T10:16:11
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
qtHaveModule(printsupport): QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeviceDiscriptionTool3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    user/login.cpp \
    user/userinformation.cpp \
    devicelistwidget.cpp \
    devicetreewidget.cpp \
    user/userinformationwidget.cpp \
    formtest.cpp \
    devices/functions/functionlistwidget.cpp \
    functionstruct.cpp \
    devices/functions/editbutton.cpp \
    mypushbutton.cpp \
    devices/functions/parameterwidget.cpp \
    devices/functions/addfunctionswidget.cpp \
    devices/deviceinformationwidget.cpp \
    devices/filegenerater.cpp \
    headinformation.cpp \
    devices/adddevicewidget.cpp \
    managefunctionwidget.cpp \
    settingwidget.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    user/login.h \
    user/userinformation.h \
    config.h \
    devicelistwidget.h \
    devicetreewidget.h \
    user/userinformationwidget.h \
    formtest.h \
    devices/functions/functionlistwidget.h \
    functionstruct.h \
    devices/functions/editbutton.h \
    mypushbutton.h \
    devices/functions/parameterwidget.h \
    devices/functions/addfunctionswidget.h \
    devices/deviceinformationwidget.h \
    devices/filegenerater.h \
    headinformation.h \
    devices/adddevicewidget.h \
    managefunctionwidget.h \
    settingwidget.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    user/login.ui \
    user/userinformationwidget.ui \
    formtest.ui \
    devices/functions/parameterwidget.ui \
    devices/functions/addfunctionswidget.ui \
    devices/deviceinformationwidget.ui \
    devices/adddevicewidget.ui \
    settingwidget.ui \
    aboutdialog.ui
UI_DIR = ./UI

RESOURCES += \
    doc.qrc
