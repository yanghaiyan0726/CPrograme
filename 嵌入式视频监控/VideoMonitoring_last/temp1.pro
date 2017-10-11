#-------------------------------------------------
#
# Project created by QtCreator 2017-05-23T19:08:51
#
#-------------------------------------------------

QT       += core gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = temp1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    showui.cpp \
    videoproduce.cpp \
    database.cpp \
    videoEncoder.cpp \
    listphoto.cpp \
    listvideo.cpp \
    showphoto.cpp \
    showvideo.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    showui.h \
    videoproduce.h \
    database.h \
    ffCodecs.hpp \
    videoEncoder.h \
    listphoto.h \
    listvideo.h \
    showphoto.h \
    showvideo.h \
    source.h

FORMS    += mainwindow.ui \
    dialog.ui \
    listphoto.ui \
    listvideo.ui \
    showphoto.ui \
    showvideo.ui

CONFIG += \
    link_pkgconfig

# 添加opencv的库
PKGCONFIG += opencv

# 添加opencv第三方库ippicv
LIBS += -L /usr/local/share/OpenCV/3rdparty/lib -lippicv
INCLUDEPATH +=./usr/local/include
LIBS += -L./usr/local/lib/ -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswscale -lswresample

RESOURCES += \
    image.qrc
