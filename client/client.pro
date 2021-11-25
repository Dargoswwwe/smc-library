TEMPLATE = app
TARGET = client

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/mainwindow.hpp

FORMS += \
    src/mainwindow.ui

# Directories
DESTDIR=bin                 #Target file directory
OBJECTS_DIR=intermediates   #Intermediate object files directory
MOC_DIR=intermediates       #Intermediate moc files directory
UI_DIR=intermediates        #Intermediate uic files directory

unix|win32: LIBS += -L$$PWD/../shared/lib/ -lsmclibrary

INCLUDEPATH += $$PWD/../shared/src
DEPENDPATH += $$PWD/../shared/src
