QT += core gui widgets
CONFIG += c++17
TARGET = clientscreen
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    multicast_receiver.cpp

HEADERS += \
    Database.h \
    mainwindow.h \
    multicast_receiver.h

FORMS +=
# Linux için FFmpeg ayarları
unix {
    #LIBS += -lqtermwidget5
    #INCLUDEPATH += /usr/include/qtermwidget5
    INCLUDEPATH += /usr/include
    LIBS += -lavformat -lavcodec -lavutil -lswscale
}

# Windows için FFmpeg ayarları (örnek: FFmpeg statik binary kurulu dizin)
win32 {
    INCLUDEPATH += C:/ffmpeg/include
    LIBS += -LC:/ffmpeg/lib \
            -lavformat \
            -lavcodec \
            -lavutil \
            -lswscale
}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
target.path = /usr/bin
INSTALLS += target
