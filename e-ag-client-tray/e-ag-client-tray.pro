#-------------------------------------------------
#
# Project created by QtCreator 2019-07-02T00:29:01
#
#-------------------------------------------------

QT       += widgets network core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = e-ag-client-tray
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    singleinstance.cpp

HEADERS += \
    ekran.h \
    ekranresmi.h \
    filecrud.h \
        mainwindow.h \
    singleinstance.h

FORMS +=

RESOURCES += \
    resources.qrc


target.path = /usr/bin

icon.files = icons/e-ag-client.svg
icon.path = /usr/share/icons

auto_start.files = e-ag-client-tray.desktop
auto_start.path = /etc/xdg/autostart/

INSTALLS += target icon auto_start
#service

#DISTFILES += \
#    e-ag-x11client.service
