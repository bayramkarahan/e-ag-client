QT -= gui
QT += network core
CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = e-ag-client
TEMPLATE = app
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    client.cpp \
    singleinstance.cpp


HEADERS += \
    client.h \
    filecrud.h \
    singleinstance.h


target.path = /usr/bin


service.files = e-ag-client.service
service.path = /lib/systemd/system/

x11servicedesktop.files = e-ag-x11vncdesktop.service
x11servicedesktop.path = /lib/systemd/system/

x11servicelogin.files = e-ag-x11vnclogin.service
x11servicelogin.path = /lib/systemd/system/


x11passwd.files = x11vncpasswd
x11passwd.path = /usr/bin/

runfile.files = e-ag-run.sh
runfile.path = /usr/bin/

webdisablefile.files = webdisable.sh
webdisablefile.path = /usr/share/e-ag/

INSTALLS += target service x11servicedesktop x11servicelogin x11passwd runfile webdisablefile


DISTFILES += \
    e-ag-client.service\
    e-ag-x11vncdesktop.service\
    e-ag-x11vnclogin.service\
    x11vncpasswd\
    e-ag-run.sh\
    webdisable.sh


