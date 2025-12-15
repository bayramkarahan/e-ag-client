QT += core gui widgets multimedia
#sudo apt install libavformat-dev libavcodec-dev libswscale-dev libavutil-dev

CONFIG += c++17

SOURCES += \
    main.cpp \
    ffmpegplayer.cpp \
    mainwindow.cpp

HEADERS += \
    ffmpegplayer.h \
    mainwindow.h \
    mainwindow1.h

INCLUDEPATH += /usr/include

LIBS += -lavformat -lavcodec -lavutil -lswresample -lswscale -lavdevice
