#-------------------------------------------------
#
# Project created by QtCreator 2019-07-02T00:29:01
#
#-------------------------------------------------

QT       += widgets network core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = e-ag-client-gui
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
CONFIG += qdbus
SOURCES += \
        hostnamedialog.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
    hostnamedialog.h \
        mainwindow.h \
    ayar.h \
    giris.h \
    hakkinda.h \
    networkWidget.h \
    webblockWidget.h

FORMS +=

RESOURCES += \
    resources.qrc


target.path = /usr/bin

desktop_file.files = e-ag-client-gui.desktop
desktop_file.path = /usr/share/applications/

icon.files = icons/e-ag-client.svg
icon.path = /usr/share/icons

#service.files = e-ag-x11client.service
#service.path = /lib/systemd/system/

INSTALLS += target desktop_file icon
#service

#DISTFILES += \
#    e-ag-x11client.service
