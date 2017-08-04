#-------------------------------------------------
#
# Project created by QtCreator 2015-12-15T13:07:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    init.cpp \
    CharString.cpp \
    MusicInfo.cpp \
    Sqstack.cpp \
    BTreeAndDoc.cpp \
    findWord.cpp

HEADERS  += mainwindow.h \
    init.h \
    compare.h \
    CharString.h \
    Const.h \
    MusicInfo.h \
    Sqstack.h \
    BTreeAndDoc.h \
    findWord.h

FORMS    += mainwindow.ui

RESOURCES += \
    Sour.qrc
