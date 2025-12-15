QT -= gui
QT += network core printsupport
CONFIG += console

CONFIG += c++11
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = e-ag-client-console
TEMPLATE = app
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    client.cpp \
    scdimgserver.cpp \
    scdimgserverthread.cpp \
    singleinstance.cpp \
    userprivilegehelper.cpp


HEADERS += \
    Database.h \
    client.h \
    scdimgserver.h \
    scdimgserverthread.h \
    singleinstance.h \
    userprivilegehelper.h


target.path = /usr/bin


service.files = e-ag-client-console.service
service.path = /lib/systemd/system/

x11servicedesktop.files = e-ag-x11vncdesktop.service
x11servicedesktop.path = /lib/systemd/system/

x11servicelogin.files = e-ag-x11vnclogin.service
x11servicelogin.path = /lib/systemd/system/

debeagscript.files = debeagscript.service
debeagscript.path = /lib/systemd/system/

eagscript.files = eagscript.service
eagscript.path = /lib/systemd/system/


cleandesktop.files = clean-desktop
cleandesktop.path = /usr/bin/


x11passwd.files = x11vncpasswd
x11passwd.path = /usr/bin/

runfile.files = e-ag-run.sh
runfile.path = /usr/bin/

webdisablefile.files = webdisable.sh
webdisablefile.path = /usr/share/e-ag/

lisanss.files = lisans/*
lisanss.path = /usr/share/doc/e-ag/LICENSES/

script.files = script/*
script.path = /usr/bin/

INSTALLS += target service x11servicedesktop x11servicelogin x11passwd \
webdisablefile lisanss script cleandesktop debeagscript eagscript


DISTFILES += \
    e-ag-client.service\
    e-ag-x11vncdesktop.service\
    e-ag-x11vnclogin.service\
    x11vncpasswd\
    webdisable.sh


