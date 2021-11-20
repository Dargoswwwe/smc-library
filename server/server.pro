TEMPLATE = app
TARGET = server
INCLUDEPATH += . ../shared/src/

QT       += core network sql

CONFIG += c++latest

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
SOURCES += \
    src/database_management.cpp \
    src/main.cpp \
    src/server.cpp

HEADERS += \
    src/database_management.hpp \
    src/server.hpp

# Directories
DESTDIR=bin                 #Target file directory
OBJECTS_DIR=intermediates   #Intermediate object files directory
MOC_DIR=intermediates       #Intermediate moc files directory
UI_DIR=intermediates        #Intermediate uic files directory

unix|win32: LIBS += -L$$PWD/../shared/lib/ -lsmclibrary

INCLUDEPATH += $$PWD/../shared/src
DEPENDPATH += $$PWD/../shared/src

DISTFILES +=
