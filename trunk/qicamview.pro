#-------------------------------------------------
#
# Project created by QtCreator 2010-12-12T12:28:04
#
#-------------------------------------------------

QT       += core gui network

TARGET = qicamview
TEMPLATE = app
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build 
DESTDIR = bin


SOURCES += main.cpp\
        mainwindow.cpp \
    icamviewgrabber.cpp \
    imagegrabber.cpp \
    dialogimagegrabbersettings.cpp

HEADERS  += mainwindow.h \
    icamviewgrabber.h \
    imagegrabber.h \
    getimage_global.h \
    dialogimagegrabbersettings.h

FORMS    += mainwindow.ui \
    dialogimagegrabbersettings.ui
